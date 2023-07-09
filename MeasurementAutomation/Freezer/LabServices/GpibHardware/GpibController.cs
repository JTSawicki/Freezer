using LabServices.Exceptions;
using Serilog;
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LabServices.GpibHardware
{
    /// <summary>
    /// Klasa kontrolera magistrali GPIB
    /// </summary>
    public sealed class GpibController : IDisposable
    {
        /// <summary>Adres kontrolera Gpib C++ w pamięci 🤣</summary>
        private IntPtr _cppController;
        /// <summary>Maksymalna wielkość bufora danych przy połączeniu. Inicjowane przy this.Start</summary>
        private int MaxBufferSize;

        /// <summary>Czy kontroler włączony</summary>
        public bool IsActive { get; private set; }
        /// <summary>Czy podłączono do urządzenia</summary>
        public bool IsConnected { get; private set; }
        /// <summary>Adres podłączonego urządzenia</summary>
        public int? DeviceAddress { get; private set; }

        public GpibController()
        {
            IsActive = false;
            IsConnected = false;
            DeviceAddress = null;
        }

        // Publiczne funkcje
        // --------------------------------------------------

        /// <summary>
        /// Uruchamia połączenie z magistralą
        /// Należy potem zamknąć -> Dispose
        /// </summary>
        public void Start()
        {
            IsActive = true;
            _cppController = CppCreateController();
            CppSesionStart(_cppController);
            CheckForError();
            MaxBufferSize = CppGetBufferSize(_cppController);
        }

        /// <summary>
        /// Nawiązuje połączenie z urządzeniem na magistrali
        /// </summary>
        /// <param name="address">Adres urządzenia</param>
        /// <exception cref="NotConnectedException"></exception>
        public void DeviceConnect(int address)
        {
            if (!IsActive)
            {
                throw new NotConnectedException("Gpib controller is not started");
            }

            DeviceAddress = address;
            string addressString = $"GPIB0::{address}::INSTR";
            CppDeviceConnect(_cppController, addressString);
            CheckForError();
            IsConnected = true;
        }

        /// <summary>
        /// Rozłącza z urządzeniem na magistrali
        /// </summary>
        public void DeviceDisconnect()
        {
            CheckIfConnected();
            CppDeviceDisconnect(_cppController);
            CheckForError();
            IsConnected = false;
        }

        /// <summary>
        /// Zapytanie do podłączonego urządzenia na magistrali
        /// </summary>
        /// <param name="command"></param>
        /// <returns></returns>
        public string Query(string command)
        {
            CheckIfConnected();
            StringBuilder sb = new StringBuilder(MaxBufferSize);
            CppQuery(_cppController, command, sb);
            CheckForError();
            return sb.ToString();
        }

        /// <summary>
        /// Zapytanie do podłączonego urządzenia na magistrali o dużym rozmiarze odpowiedzi
        /// </summary>
        /// <param name="command"></param>
        /// <param name="maxResponseSize">Maksymalny rozmiar odpowiedzi</param>
        /// <returns></returns>
        public string QueryBigResponse(string command, int maxResponseSize)
        {
            CheckIfConnected();
            StringBuilder sb = new StringBuilder(maxResponseSize);
            CppBigResponseQuery(_cppController, command, sb, maxResponseSize);
            try
            {
                CheckForError();
            } catch (Exception ex)
            {
                Log.Error("Error on QueryBigResponse", ex);
                throw new Exception("Error on QueryBigResponse");
            }
            return sb.ToString();
        }

        /// <summary>
        /// Zapis do podłączonego urządzenia na magistrali
        /// </summary>
        /// <param name="command"></param>
        public void Write(string command)
        {
            CheckIfConnected();
            StringBuilder sb = new StringBuilder(MaxBufferSize);
            CppWrite(_cppController, command);
            CheckForError();
        }

        /// <summary>
        /// Odczyt z podłączonego urządzenia na magistrali
        /// </summary>
        /// <returns></returns>
        public string Read()
        {
            CheckIfConnected();
            StringBuilder sb = new StringBuilder(MaxBufferSize);
            CppRead(_cppController, sb);
            CheckForError();
            return sb.ToString();
        }

        /// <summary>
        /// Włącza obsługę eventu SRQ
        /// </summary>
        public void EnableSRQ()
        {
            CppEnableSRQ(_cppController);
        }

        /// <summary>
        /// Oczekuje na event SRQ
        /// </summary>
        /// <param name="waitTimeout">Timeout oczekiwania</param>
        public void WaitForSRQ(int waitTimeout)
        {
            CppWaitForSRQ(_cppController, waitTimeout);
        }

        // Prywatne funkcje
        // --------------------------------------------------

        /// <summary>
        /// Sprawdza czy biblioteka w C++ rzuciła błędy i rzuca je w C# jeżeli tak
        /// </summary>
        /// <exception cref="CppGpibException"></exception>
        private void CheckForError()
        {
            bool hasError = CppHasError(_cppController);
            if (hasError)
            {
                StringBuilder sb = new StringBuilder(MaxBufferSize);
                CppGetError(_cppController, sb);
                throw new CppGpibException(sb.ToString());
            }
        }

        /// <summary>
        /// Sprawdza podłączono do urządzenia
        /// </summary>
        /// <exception cref="NotConnectedException"></exception>
        private void CheckIfConnected()
        {
            if (!IsActive)
            {
                throw new NotConnectedException("Gpib controller is not started");
            }
            if (!IsConnected)
            {
                throw new NotConnectedException("Gpib controller is not connected to any device");
            }
        }

        /// <summary>
        /// Zwalnia zasoby połączenia
        /// </summary>
        public void Dispose()
        {
            if (IsActive && IsConnected)
            {
                CppDeviceDisconnect(_cppController);
                CppSesionStop(_cppController);
            }
            if (IsActive)
            {
                CppSesionStop(_cppController);
            }
        }

        // Dll imports
        // --------------------------------------------------

        [DllImport("GpibCppController.dll")]
        private static extern IntPtr CppCreateController();

        [DllImport("GpibCppController.dll")]
        private static extern void CppSesionStart(IntPtr obj);

        [DllImport("GpibCppController.dll")]
        private static extern void CppSesionStop(IntPtr obj);

        [DllImport("GpibCppController.dll")]
        private static extern void CppDeviceConnect(IntPtr obj, string address);

        [DllImport("GpibCppController.dll")]
        private static extern void CppDeviceDisconnect(IntPtr obj);

        [DllImport("GpibCppController.dll")]
        private static extern int CppGetBufferSize(IntPtr obj);

        [DllImport("GpibCppController.dll")]
        private static extern bool CppHasError(IntPtr obj);

        [DllImport("GpibCppController.dll")]
        private static extern void CppGetError(IntPtr obj, StringBuilder output);

        [DllImport("GpibCppController.dll")]
        private static extern void CppWrite(IntPtr obj, string command);

        [DllImport("GpibCppController.dll")]
        private static extern void CppRead(IntPtr obj, StringBuilder response);

        [DllImport("GpibCppController.dll")]
        private static extern void CppQuery(IntPtr obj, string command, StringBuilder response);

        [DllImport("GpibCppController.dll")]
        private static extern void CppBigResponseQuery(IntPtr obj, string command, StringBuilder response, int responseSize);

        [DllImport("GpibCppController.dll")]
        private static extern void CppEnableSRQ(IntPtr obj);

        [DllImport("GpibCppController.dll")]
        private static extern void CppWaitForSRQ(IntPtr obj, int waitTimeout);
    }
}
