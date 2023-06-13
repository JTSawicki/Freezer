using LabServices.GpibHardware;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Testowa_Konsola.Tests
{
    internal class Kithley2400_ConcurrentTestSweep : ConsoleTest
    {
        public Kithley2400_ConcurrentTestSweep() : base("Kithley2400 Concurrent Test Sweep", "Wykonuje sweep na układzie testowym MaxV=10V MaxI=0.01A")
        {
        }

        private bool _eventFlag = false;
        private object _eventFlagLock = new object();

        public override void ExecuteTest()
        {
            GpibHardwareInitData hardwareInitData = new GpibHardwareInitData()
            {
                KithleyAddress = 24,
                KithleyConnected = true,
                LakeShoreAddress = 23,
                LakeShoreConnected = false,
            };
            KithleySweeperInitData sweeperInitData = new KithleySweeperInitData()
            {
                ConnectionType = Kithley.ConnectionType.TwoWireTerminal,
                VoltageSegmentation = Kithley.VoltageSegmentation.Logarytmic,
                VoltageStart = "1",
                VoltageStop = "10",
                // VoltageStep = "1",
                CurrentProtection = "0.01",
                VoltagePoints = 10,
                SourceDelay = 0.05
            };

            WriteLine("Uruchamianie kontrolera");
            GpibHardwareController controller = new GpibHardwareController();
            controller.StartController(hardwareInitData);

            controller.NewMeasuermentEvent += Controller_NewMeasuermentEvent;

            Thread.Sleep(500);
            WriteLine("Zlecanie sweep");
            controller.PushCommand(GpibCommands.Sweep, new List<object> { sweeperInitData });

            WriteLine("Oczekiwanie na wynik");
            bool flag;
            do
            {
                lock (_eventFlagLock)
                {
                    flag = !_eventFlag;
                }
                Thread.Sleep(100);
            } while (flag);

            List<KithleyMeasurement> measurement = new List<KithleyMeasurement>();
            do
            {
                measurement = controller.TryGetLastMeasurements(1);
            } while (measurement.Count < 1);
            WriteLine($"Wynik działania:\n{measurement[0]}");

            WriteLine("Wyłączanie kontrolera");
            controller.StopController();
            while (controller.IsActive())
            {
                Thread.Sleep(100);
            }
            WriteLine("Wyłączono kontroler");
        }

        private void Controller_NewMeasuermentEvent(object? sender, EventArgs e)
        {
            lock(_eventFlagLock)
            {
                _eventFlag = true;
            }
        }
    }
}
