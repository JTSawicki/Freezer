using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LabServices.GpibHardware;

namespace Testowa_Konsola.Tests
{
    internal class Kithley2400_Query : ConsoleTest
    {
        public Kithley2400_Query() : base("Kithley 2400 query", "Proste zapytanie o identyfikację sprawdzające magistralę GPIB")
        {
            ;
        }

        public override void ExecuteTest()
        {
            // string resourceName = "GPIB0::24::INSTR";
            int kithleyAddress = 16;
            string command = "*IDN?";
            string result = "";

            Write($"Adres urządzenia: {kithleyAddress}\n");
            Write($"Komenda: {command}\n");

            try
            {
                GpibController controller = new GpibController();
                controller.Start();
                controller.DeviceConnect( kithleyAddress );
                result = controller.Query(command);
                controller.Dispose();

                Write($"Odpowiedź: {result}\n");
            }
            catch(Exception e)
            {
                Write($"Błąd wykonania: {e.Message}\n");
            }
            Write("Koniec testu\n");
        }
    }
}
