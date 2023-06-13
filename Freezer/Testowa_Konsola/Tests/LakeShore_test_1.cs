using LabServices.GpibHardware;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testowa_Konsola.Tests
{
    public class LakeShore_test_1 : ConsoleTest
    {
        public LakeShore_test_1() : base("LakeShore Test 1", "Testowe połączenie z lakeshore") { }

        public override void ExecuteTest()
        {
            string command = "*IDN?";
            GpibController controller = new GpibController();
            controller.Start();

            while (command != "exit")
            {
                try
                {
                    Stopwatch stopwatch_1 = Stopwatch.StartNew(); // ~30ms, ~80ms przy rozruchu
                    controller.DeviceConnect(16);
                    Stopwatch stopwatch_2 = Stopwatch.StartNew(); // ~20ms
                    string result = controller.Query(command);
                    controller.DeviceDisconnect();
                    stopwatch_1.Stop();
                    stopwatch_2.Stop();

                    WriteLine($"Odpowiedź: {result}");
                    WriteLine($"Czas zapytania: {stopwatch_1.ElapsedMilliseconds}, {stopwatch_2.ElapsedMilliseconds}");
                }
                catch (Exception e)
                {
                    Write($"Błąd wykonania: {e.Message}\n");
                }
                Write("Podaj komendę: ");
                command = ReadLine()!.Trim();
            }

            controller.Dispose();
            WriteLine("Koniec testu");
        }
    }
}
