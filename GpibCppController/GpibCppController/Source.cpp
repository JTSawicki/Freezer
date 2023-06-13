#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
/* Functions like strcpy are technically not secure because they do */
/* not contain a 'length'. But we disable this warning for the VISA */
/* examples since we never copy more than the actual buffer size.   */
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <iostream>
#include <cstring>
#include <string>
#include "visa.h"
//#include <chrono>
//#include <sstream>

/// <summary>Wspó³czynnik czasu przesy³ania 1 znaku w milisekundach</summary>
#define timeoutCoeffitent  0.8f;

class Controller {
private:
	/// <summary>Czy kontroler jest aktywny</summary>
	bool isActive;
	/// <summary>Czy do kontrolera jest podpiête urz¹dzenie</summary>
	bool isConnected;

	/// <summary>Czy s¹ b³êdy</summary>
	bool errorState;
	/// <summary>Maksymalna wielkoœæ bufora b³êdu</summary>
	static const int defaultBufferSize = 500;
	/// <summary>Bufor b³êdu</summary>
	char* errorBuffer;

	/// <summary>Timeout sesji urz¹dzenia</summary>
	static const int timeout = defaultBufferSize * timeoutCoeffitent;

	ViSession defaultRM;
	ViSession instr;
	ViStatus status;
	ViUInt32 retCount;
	ViUInt32 writeCount;
	ViEvent ehandle;
	ViEventType etype;

	/// <summary>
	/// Ustawia b³¹d
	/// </summary>
	/// <param name="message">Treœæ b³êdu</param>
	void ThrowError(std::string message) {
		memset(errorBuffer, '\0', defaultBufferSize);
		strcpy_s(errorBuffer, defaultBufferSize, message.c_str());
		errorState = true;
	}

	/// <summary>
	/// Ustawia timeout dla odpowiedzi na magistrali Gpib
	/// </summary>
	/// <param name="value"></param>
	void SetTimeout(int value) {
		status = viSetAttribute(instr, VI_ATTR_TMO_VALUE, value);
		if (status < VI_SUCCESS) {
			ThrowError("Cannot set timeout of 100ms in device session\n");
			return;
		}
	}

public:
	Controller() {
		isActive = false;
		isConnected = false;
		errorState = false;

		errorBuffer = new char[defaultBufferSize];
		std::memset(errorBuffer, '\0', defaultBufferSize);
	}

	~Controller() {
		if (isConnected)
			DeviceDisconnect();
		if (isActive)
			SessionStop();
		delete[] errorBuffer;
	}

	/// <summary>
	/// Uruchamia kontroler GPIB
	/// </summary>
	void SessionStart() {
		status = viOpenDefaultRM(&defaultRM);
		if (status < VI_SUCCESS) {
			ThrowError("Could not open VISA Resource Manager session\n");
			return;
		}
		isActive = true;
	}

	/// <summary>
	/// Roz³¹cza kontroler GPIB
	/// </summary>
	void SessionStop() {
		if (isActive) {
			if (isConnected) {
				ThrowError("Could not close VISA Resource Manager session because connected to instrument\n");
				return;
			}
			status = viClose(defaultRM);
			if (status < VI_SUCCESS) {
				ThrowError("Cannot close VISA Resource Manager session\n");
				return;
			}
			isActive = false;
		}
	}

	/// <summary>
	/// Nawi¹zuje po³¹czenie z urz¹dzeniem o podanym adresie
	/// </summary>
	/// <param name="address">Przyk³adowy adres -> GPIB0::2::INSTR</param>
	void DeviceConnect(const char* address) {
		if (isConnected) {
			DeviceDisconnect();
		}

		status = viOpen(defaultRM, address, VI_NULL, VI_NULL, &instr);
		if (status < VI_SUCCESS) {
			ThrowError("Cannot open device session\n");
			return;
		}

		SetTimeout(timeout);
	}

	/// <summary>
	/// Roz³¹cza z urz¹dzeniem
	/// </summary>
	void DeviceDisconnect() {
		if (isConnected) {
			status = viClose(instr);
			if (status < VI_SUCCESS) {
				ThrowError("Cannot close device session\n");
				return;
			}
		}
	}

	/// <summary>
	///Maksymalny rozmiar bufora poleceñ i b³êdów
	/// </summary>
	/// <returns></returns>
	inline int GetBufferSize() {
		return this->defaultBufferSize;
	}

	/// <summary>
	/// Czy s¹ b³êdy
	/// </summary>
	/// <returns></returns>
	inline int HasError() {
		return this->errorState;
	}

	/// <summary>
	/// Zwraca treœæ b³êdu
	/// </summary>
	/// <param name="output">£¹cze do obiektu wyjœciowego</param>
	inline void GetError(char* output) {
		strcpy_s(output, this->defaultBufferSize, this->errorBuffer);
		errorState = false;
		memset(errorBuffer, '\0', defaultBufferSize);
	}

	/// <summary>
	/// Zapisuje komendê do urz¹dzenia
	/// </summary>
	/// <param name="message"></param>
	inline void Write(const char* message) {
		status = viWrite(instr, (ViBuf)message, (ViUInt32)strlen(message), &writeCount);
		if (status < VI_SUCCESS)
		{
			ThrowError("Error writing to the device\n");
			return;
		}
	}

	/// <summary>
	/// Czyta odpowiedŸ urz¹dzenia
	/// </summary>
	/// <param name="output">WskaŸnik na tablicê wyjœciow¹</param>
	/// <param name="outputSize">Rozmiar wyjœcia</param>
	inline void Read(char* output, int outputSize = defaultBufferSize) {
		unsigned char* readBuffer = new unsigned char[outputSize];
		std::memset(readBuffer, '\0', outputSize);
		status = viRead(instr, readBuffer, outputSize, &retCount);
		if (status < VI_SUCCESS)
		{
			ThrowError("Error reading a response from the device\n");
			delete[] readBuffer;
			return;
		}
		strcpy_s(output, outputSize, (char*)readBuffer);
		delete[] readBuffer;
	}

	/// <summary>
	/// Tworzy zapytanie dla urz¹dzenia
	/// </summary>
	/// <param name="message"></param>
	/// <param name="output"></param>
	inline void Query(const char* message, char* output) {
		Write(message);
		Read(output);
	}

	/// <summary>
	/// Tworzy zapytanie dla urz¹dzenia
	/// </summary>
	/// <param name="message"></param>
	/// <param name="output"></param>
	/// <param name="outputSize"></param>
	inline void BigResponseQuery(const char* message, char* output, int outputSize) {
		Write(message);
		//auto start = std::chrono::steady_clock::now();
		int tmpTimeout = outputSize * timeoutCoeffitent;
		SetTimeout(tmpTimeout);
		Read(output, outputSize);
		SetTimeout(timeout);
		//auto stop = std::chrono::steady_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
		//std::ostringstream out;
		//out << duration << " ms\n";
		//ThrowError(out.str());
	}

	/// <summary>
	/// Aktywuje event SRQ
	/// </summary>
	inline void EnableSRQ() {
		status = viEnableEvent(instr, VI_EVENT_SERVICE_REQ, VI_QUEUE, VI_NULL);
		if (status < VI_SUCCESS)
		{
			ThrowError("The SRQ event could not be enabled");
		}
	}

	/// <summary>
	/// Oczekuje na event SRQ
	/// </summary>
	inline void WaitForSRQ(int waitTimeout) {
		viWaitOnEvent(instr, VI_EVENT_SERVICE_REQ, waitTimeout, &etype, &ehandle);
	}
};


//// Testowy main
//int main() {
//	Controller c = Controller();
//	std::string device = "GPIB0::24::INSTR";
//	std::string command = "*IDN?";
//	char* result = new char[c.GetBufferSize()];
//	std::memset(result, '\0', c.GetBufferSize());
//
//	c.SessionStart();
//	c.DeviceConnect(device.c_str());
//	c.Query(command.c_str(), result);
//	std::cout << result << std::endl;
//	std::memset(result, '\0', c.GetBufferSize());
//	c.Write(command.c_str());
//	c.Read(result);
//	std::cout << result << std::endl;
//	c.DeviceDisconnect();
//	c.SessionStop();
//
//	delete[] result;
//	return EXIT_SUCCESS;
//}

//int main() {
//	static ViEvent ehandle;
//	static ViEventType etype;
//
//	Controller controller = Controller();
//	int bufferSize = controller.GetBufferSize();
//
//	char* buffer = new char[bufferSize];
//	std::memset(buffer, '\0', bufferSize);
//
//	std::string tmpString = "";
//
//	controller.SessionStart();
//	controller.DeviceConnect("GPIB0::24::INSTR");
//
//	controller.Write("*RST");
//	controller.Write("*CLS");
//
//	//controller.Write("*ESE 1");
//	//controller.Write("*SRE 32");
//
//	controller.Write(":SENS:FUNC:CONC OFF");
//	controller.Write(":SOUR:FUNC VOLT");
//	controller.Write(":SENS:FUNC 'CURR:DC'");
//	controller.Write(":SENS:CURR:PROT 0.01");
//
//	controller.Write(":SOUR:VOLT:START 1");
//	controller.Write(":SOUR:VOLT:STOP 5");
//	controller.Write(":SOUR:VOLT:STEP 1");
//	controller.Write(":SOUR:VOLT:MODE SWE");
//	controller.Write(":SOUR:DEL 0.05");
//	controller.Write(":FORM:ELEM VOLT,CURR");
//
//	controller.Query(":SOUR:SWE:POIN?", buffer);
//	std::cout << "Point count: " << buffer << std::endl;
//
//	std::string points = buffer;
//	tmpString = ":TRIGger:COUNt " + points;
//	std::cout << tmpString << std::endl;
//	controller.Write(tmpString.c_str());
//
//	// Aktywacja eventu
//	controller.Write(":STAT:OPER:ENAB 1024");
//	controller.Write("*SRE 128");
//	controller.EnableSRQ();
//
//	// Uruchamianie pomiaru
//	controller.Write(":OUTPUT ON");
//	controller.Write(":INIT");
//	controller.WaitForSRQ(100000);
//
//	// Pobieranie danych z pomiaru
//	int bigBufferSize = 14 * 2 * std::stoi(points) + 10;
//	char* bigBuffer = new char[bigBufferSize];
//	std::memset(bigBuffer, '\0', bigBufferSize);
//	controller.BigResponseQuery(":FETCH?", bigBuffer, bigBufferSize);
//	std::cout << "Wynik: " << bigBuffer << std::endl;
//
//	// Wy³¹czenie urz¹dzenia
//	controller.Write(":OUTPUT OFF");
//	controller.DeviceDisconnect();
//	controller.SessionStop();
//
//	delete[] buffer;
//	return EXIT_SUCCESS;
//}

extern "C" __declspec(dllexport) void* __stdcall CppCreateController() {
	return (void*) new Controller();
}

extern "C" __declspec(dllexport) void __stdcall CppSesionStart(Controller * obj) {
	obj->SessionStart();
}

extern "C" __declspec(dllexport) void __stdcall CppSesionStop(Controller * obj) {
	obj->SessionStop();
}

extern "C" __declspec(dllexport) void __stdcall CppDeviceConnect(Controller * obj, const char* address) {
	obj->DeviceConnect(address);
}

extern "C" __declspec(dllexport) void __stdcall CppDeviceDisconnect(Controller * obj) {
	obj->DeviceDisconnect();
}

extern "C" __declspec(dllexport) int __stdcall CppGetBufferSize(Controller * obj) {
	return obj->GetBufferSize();
}

extern "C" __declspec(dllexport) bool __stdcall CppHasError(Controller * obj) {
	return obj->HasError();
}

extern "C" __declspec(dllexport) void __stdcall CppGetError(Controller * obj, char* output) {
	obj->GetError(output);
}

extern "C" __declspec(dllexport) void __stdcall CppWrite(Controller * obj, const char* command) {
	obj->Write(command);
}

extern "C" __declspec(dllexport) void __stdcall CppRead(Controller * obj, char* response) {
	obj->Read(response);
}

extern "C" __declspec(dllexport) void __stdcall CppQuery(Controller * obj, const char* command, char* response) {
	obj->Query(command, response);
}

extern "C" __declspec(dllexport) void __stdcall CppBigResponseQuery(Controller * obj, const char* command, char* response, int responseSize) {
	obj->BigResponseQuery(command, response, responseSize);
}

extern "C" __declspec(dllexport) void __stdcall CppEnableSRQ(Controller * obj) {
	obj->EnableSRQ();
}

extern "C" __declspec(dllexport) void __stdcall CppWaitForSRQ(Controller * obj, int waitTimeout) {
	obj->WaitForSRQ(waitTimeout);
}