using LabServices.GpibHardware;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Testowa_Konsola.Tests
{
    public class Kithley2400_TestSweep : ConsoleTest
    {
        public Kithley2400_TestSweep() :base("Kithley2400 Test Sweep", "Wykonuje sweep na układzie testowym MaxV=5V MaxI=0.01A") { }

        public override void ExecuteTest()
        {
            int adress = 24;
            List<string> result = new List<string>();
            GpibController controller = new GpibController();

            // Ustawienia testu
            // Lista nie działa
            SweepMode mode = SweepMode.Logarytmic;

            // Zmienne wywołania
            int numberOfPoints = 10;

            WriteLine($"Uruchamianie kontrolera z połączeniem na adresie{adress}");
            controller.Start();
            controller.DeviceConnect(adress);

            WriteLine($"Podłączono do: {controller.Query("*IDN?")}");

            // Zerowanie połączenia
            controller.Write("*RST");
            controller.Write("*CLS");

            // Ustawienia połączenia
            /*controller.Write("*ESE 1");
            controller.Write("*SRE 32");*/

            // Ustawienia sweep
            controller.Write("SENS:FUNC:CONC OFF");
            controller.Write("SOUR:FUNC VOLT");
            controller.Write("SENS:FUNC 'CURR:DC'");
            controller.Write("SENS:CURR:PROT 0.01");
            controller.Write("SYST:RSEN OFF"); // Akrywacja Remote-Sense -> pomiar 4 kable

            if(mode == SweepMode.Linear)
            {
                controller.Write("SOURce:SWEep:SPACing LIN");
                controller.Write("SOUR:VOLT:START 1");
                controller.Write("SOUR:VOLT:STOP 10");
                controller.Write("SOUR:VOLT:STEP 0.9");

                numberOfPoints = int.Parse(controller.Query("SOUR:SWE:POIN?"));
                WriteLine($"Punkty sweep: {numberOfPoints}");

                controller.Write("SOUR:VOLT:MODE SWE");
                controller.Write("SOURce:SWEep:DIRection UP");
            }
            else if(mode == SweepMode.Logarytmic)
            {
                controller.Write("SOURce:SWEep:SPACing LOG");
                controller.Write("SOUR:VOLT:START -1");
                controller.Write("SOUR:VOLT:STOP -5");
                controller.Write($"TRIGger:COUNt {numberOfPoints}");

                numberOfPoints = 10;
                controller.Write($"SOUR:SWE:POIN {numberOfPoints}");
                WriteLine($"Punkty sweep: {numberOfPoints}");

                controller.Write("SOUR:VOLT:MODE SWE");
                controller.Write("SOURce:SWEep:DIRection UP");
                controller.Write($"TRIGger:COUNt {numberOfPoints}");
            }
            else if( mode == SweepMode.List)
            {
                controller.Write("SOUR:LIST:VOLT 1,4,7,9");

                numberOfPoints = int.Parse(controller.Query("SOUR:LIST:VOLT:POIN?"));
                WriteLine($"Punkty sweep: {numberOfPoints}");

                controller.Write("SOURce:VOLT:MODE LIST");
            }
            controller.Write("SOUR:DEL 0.05");
            controller.Write("FORM:ELEM VOLT,CURR");
            controller.Write($"TRIGger:COUNt {numberOfPoints}");

            // Aktywacja eventu srq
            controller.Write("STAT:OPER:ENAB 1024");
            controller.Write("*SRE 128");
            controller.EnableSRQ();

            string output = "";
            try
            {
                controller.Write("OUTPUT ON");
                controller.Write("INIT");
                controller.Write("*OPC");

                WriteLine("Wykonywanie sweep...");
                // Thread.Sleep(100 * numberOfPoints);
                controller.WaitForSRQ(500 * numberOfPoints);

                output = controller.QueryBigResponse("FETCH?", (numberOfPoints * 2 * 14) + 10);
                WriteLine($"Wynik pomiaru: {output}");
            }
            catch(Exception ex)
            {
                WriteLine($"Błąd: {ex.Message}");
            }
            finally
            {
                controller.Write("OUTPUT OFF");
            }


            string xd = output;

            Write("Wyłączenie kontrolera");
            controller.DeviceDisconnect();
            controller.Dispose();
        }

        enum SweepMode
        {
            Linear,
            Logarytmic,
            List
        }
    }
}
