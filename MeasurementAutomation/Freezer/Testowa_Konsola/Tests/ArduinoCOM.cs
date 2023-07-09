using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SerialPort = System.IO.Ports.SerialPort;


namespace Testowa_Konsola.Tests
{
    public class ArduinoCOM : ConsoleTest
    {
        public ArduinoCOM() : base("Arduino COM", "prosty test komunikacji z arduino")
        {
            ;
        }

        public override void ExecuteTest()
        {
            List<string> ComList = new List<string>(SerialPort.GetPortNames());
            WriteLine("Dostępne porty COM:");
            WriteLine("[" + string.Join(", ", ComList.ToArray()) + "]");

            if (ComList.Count == 0)
            {
                WriteLine("Brak portów do wybrania\nKończenie testu");
                return;
            }

            WriteLine("Wybierz port:");
            string port = ReadLine()!;

            SerialPort _serial;

            _serial = new SerialPort(port, 115200);
            _serial.Open();

            WriteLine("Wysyłanie komendy: 01");
            _serial.WriteLine("01");

            WriteLine("Timeout dla odpowiedzi: 1000 ms");
            Stopwatch sw = Stopwatch.StartNew();
            string response = "";
            while (response.Equals("") && sw.ElapsedMilliseconds < 1000)
            {
                if (_serial.BytesToRead > 0)
                {
                    response = _serial.ReadLine();
                }
            }
            sw.Stop();
            _serial.Close();
            WriteLine($"Odpowiedź z portu: {response}");

            WriteLine("Koniec testu");
        }
    }
}
