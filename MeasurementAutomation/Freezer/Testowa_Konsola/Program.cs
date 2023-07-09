global using System;
global using System.Collections.Generic;
global using static System.Console;

using Testowa_Konsola;
using Testowa_Konsola.Tests;
using Serilog;
using Serilog.Exceptions;
using System.Linq;

namespace MyApp // Note: actual namespace depends on the project name.
{
    internal class Program
    {
        static List<ConsoleTest> tests = new List<ConsoleTest>
        {
            new DummyTest(),
            new Kithley2400_Query(),
            new Kithley2400_TestSweep(),
            new Kithley2400_ConcurrentTestSweep(),
            new ArduinoCOM(),
            new FlowSensorIdentification(),
            new LakeShore_test_1(),
        };

        static void Main(string[] args)
        {
            SetupLog();
            WriteLine("Program testów konsolowych");
            WriteLine(
                string.Concat(Enumerable.Repeat("-", 100))
                );
            while (true)
            {
                Write("Wpisz ");
                ForegroundColor = ConsoleColor.Green;
                Write("nazwę testu");
                ResetColor();
                Write(" lub ");
                ForegroundColor = ConsoleColor.Red;
                Write("exit");
                ResetColor();
                WriteLine(" aby zakończyć");

                WriteLine("Dostępne testy:");
                for (int i = 0; i < tests.Count; i++)
                {
                    WriteLine("\t" + $"{i.ToString("00")} - " + tests[i].TestName);
                    ForegroundColor = ConsoleColor.DarkBlue;
                    WriteLine("\t\t" + tests[i].TestDescription);
                    ResetColor();
                }

                Write("Wybrany test: ");
                string choosenTest = ReadLine() ?? "";
                if (choosenTest == "exit")
                    return;

                ConsoleTest? test = FindTestByNumber(choosenTest);
                if (test != null)
                {
                    test?.ExecuteTest();
                    WriteLine(
                        string.Concat(Enumerable.Repeat("-", 50))
                        );
                }
                else
                    WriteLine("Nie wybrano poprawnego testu");
            }
        }

        static ConsoleTest? FindTestByName(string name)
        {
            foreach (ConsoleTest test in tests)
                if (test.TestName == name)
                    return test;
            return null;
        }

        static ConsoleTest? FindTestByNumber(string name)
        {
            int testNumber;
            bool success = int.TryParse(name, out testNumber);

            if (!success || testNumber >= tests.Count)
                return null;

            return tests[testNumber];
        }

        static void SetupLog()
        {
            Log.Logger = new LoggerConfiguration()
                .Enrich.WithExceptionDetails()
                .Enrich.WithThreadId()
                .Enrich.WithThreadName()
                .MinimumLevel.Verbose()
                .WriteTo.Debug(outputTemplate: "[Serilog] {Timestamp:yyyy-MM-dd HH:mm:ss.fff zzz} [{Level:u3}] {Message:lj}{NewLine}{Exception} {Properties:j}{NewLine}")
                .WriteTo.Console(outputTemplate: "[Serilog] {Timestamp:yyyy-MM-dd HH:mm:ss.fff zzz} [{Level:u3}] {Message:lj}{NewLine}{Exception} {Properties:j}{NewLine}")
                .CreateLogger();
        }
    }
}