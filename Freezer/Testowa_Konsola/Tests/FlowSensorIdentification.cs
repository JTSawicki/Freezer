using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SerialPort = System.IO.Ports.SerialPort;

namespace Testowa_Konsola.Tests
{
    public class FlowSensorIdentification : ConsoleTest
    {
        public FlowSensorIdentification() : base("Identyfikacja Czujnika Przepływu", "Identyfikuje podpięty czujnik przepływu")
        {
            ;
        }
        public override void ExecuteTest()
        {
            WriteLine("Identyfikowanie");
            List<string> comList = new List<string>(SerialPort.GetPortNames());
            comList.Sort();
            WriteLine("Dostępne porty COM:");
            WriteLine("[" + string.Join(", ", comList.ToArray()) + "]");

            if (comList.Count == 0)
            {
                WriteLine("Brak portów do wybrania\nKończenie testu");
                return;
            }

            int baudrate = 115200;
            int timeout = 100;
            string command = "01";
            string goodResponse = "ESP32-Flow";
            WriteLine($"Baudrate: {baudrate}");
            WriteLine($"Timeout: {timeout} ms");
            WriteLine($"Komenda: '{command}', oczekiwana odpowiedź: '{goodResponse}'");

            foreach (string com in comList)
            {
                SerialPort _serial = new SerialPort(com, baudrate);
                _serial.Open();
                _serial.WriteLine(command);

                Stopwatch sw = Stopwatch.StartNew();
                string response = "";
                while (response.Equals("") && sw.ElapsedMilliseconds < timeout)
                {
                    if (_serial.BytesToRead > 0)
                    {
                        response = _serial.ReadLine().Trim();
                    }
                }
                sw.Stop();
                _serial.Close();

                if (response.Equals(goodResponse))
                {
                    WriteLine($"Znaleziono na porcie: {com}");
                    break;
                }
            }

            Console.WriteLine("Kończenie testu");
        }
    }
}
