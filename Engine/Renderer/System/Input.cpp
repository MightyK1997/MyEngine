#include "Input.h"

#include <assert.h>
#include <stdint.h>

#include <Windows.h>

#include "..\BasicTypes.h"
#include "..\..\Logging\ConsolePrint.h"

namespace Engine {
	namespace Renderer
	{
		KeyStateChange		m_pKeyStateChangeCallback = nullptr;
		KeyTypes returnVal = KeyTypes::Null;

		void SetKeyStateChangeCallback(KeyStateChange i_Callback)
		{
			m_pKeyStateChangeCallback = i_Callback;
		}

		namespace Input
		{
			const size_t	m_bytesInitReadBuffer = 128;

			size_t			m_bytesReadBuffer = 0;
			LPBYTE			m_pReadBuffer = nullptr;

			const size_t	m_sizeReadTable = 256;
			uint8_t			m_VKeyStates[m_sizeReadTable];

			void ResizeReadBuffer(size_t i_bytes)
			{
				assert(i_bytes);

				if (i_bytes > m_bytesReadBuffer)
				{
					if (m_pReadBuffer)
						delete[] m_pReadBuffer;

					m_pReadBuffer = new BYTE[i_bytes];
					assert(m_pReadBuffer);

					m_bytesReadBuffer = i_bytes;
				}
			}

			bool Initialize()
			{
				assert(m_pReadBuffer == nullptr);

				ResizeReadBuffer(m_bytesInitReadBuffer);

				RAWINPUTDEVICE keyboard;

				keyboard.usUsagePage = 0x01;		// Generic desktop page
				keyboard.usUsage = 0x06;			// Keyboard
				keyboard.dwFlags = RIDEV_NOLEGACY;	// Turn off keyboard input to the Windows message handlers
				keyboard.hwndTarget = NULL;

				BOOL success = RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard));
				assert(success == TRUE);

				memset(m_VKeyStates, sizeof(m_VKeyStates), 0);

				return true;
			}

			void Service(HRAWINPUT i_Input)
			{
				UINT bytesData = 0;
				UINT ret = GetRawInputData(i_Input, RID_INPUT, NULL, &bytesData, sizeof(RAWINPUTHEADER));
				assert(ret == 0);

				if (bytesData)
				{
					ResizeReadBuffer(bytesData);
					assert(m_pReadBuffer);

					ret = GetRawInputData(i_Input, RID_INPUT, m_pReadBuffer, &bytesData, sizeof(RAWINPUTHEADER));
					assert(ret == bytesData);

					RAWINPUT * pRawInput = reinterpret_cast<RAWINPUT *>(m_pReadBuffer);
					if (pRawInput->data.keyboard.Flags == RI_KEY_MAKE)
					{
						SetKeyCode(pRawInput);
					}

					if (pRawInput->header.dwType == RIM_TYPEKEYBOARD)
					{
						if (pRawInput->data.keyboard.Flags == RI_KEY_BREAK)
						{ // up
							bool bChanged = m_VKeyStates[pRawInput->data.keyboard.VKey] != 0;

							m_VKeyStates[pRawInput->data.keyboard.VKey] = 0;

							if (bChanged)
							{
								if (m_pKeyStateChangeCallback)
									m_pKeyStateChangeCallback(pRawInput->data.keyboard.VKey, false);

								Engine::ConsolePrint_Windows("Key 0x%02x changed state to UP\n", pRawInput->data.keyboard.VKey, __FILE__, "Engine");
							}
						}
						else if (pRawInput->data.keyboard.Flags == RI_KEY_MAKE)
						{ // down
							bool bChanged = m_VKeyStates[pRawInput->data.keyboard.VKey] != 1;

							m_VKeyStates[pRawInput->data.keyboard.VKey] = 1;

							if (bChanged)
							{
								if (m_pKeyStateChangeCallback)
									m_pKeyStateChangeCallback(pRawInput->data.keyboard.VKey, true);

								Engine::ConsolePrint_Windows("Key 0x%02x changed state to DOWN\n", pRawInput->data.keyboard.VKey, __FILE__, "Engine");
							}
						}
						else
						{
							Engine::ConsolePrint_Windows("Key 0x%02x Flags: 0x%04x\n", pRawInput->data.keyboard.VKey,  __FILE__, "Engine");
						}
					}
				}

			}

			void Shutdown()
			{
				if (m_pReadBuffer)
					delete[] m_pReadBuffer;
			}

			void SetKeyCode(RAWINPUT* pRawInput)
			{
				if (pRawInput->data.keyboard.VKey == 0x41)
				{
					returnVal = KeyTypes::A;
				}
				else if (pRawInput->data.keyboard.VKey == 0x42)
				{
					returnVal = KeyTypes::B;
				}
				else if (pRawInput->data.keyboard.VKey == 0x43)
				{
					returnVal = KeyTypes::C;
				}
				else if (pRawInput->data.keyboard.VKey == 0x44)
				{
					returnVal = KeyTypes::D;
				}
				else if (pRawInput->data.keyboard.VKey == 0x45)
				{
					returnVal = KeyTypes::E;
				}
				else if (pRawInput->data.keyboard.VKey == 0x46)
				{
					returnVal = KeyTypes::F;
				}
				else if (pRawInput->data.keyboard.VKey == 0x47)
				{
					returnVal = KeyTypes::G;
				}
				else if (pRawInput->data.keyboard.VKey == 0x48)
				{
					returnVal = KeyTypes::H;
				}
				else if (pRawInput->data.keyboard.VKey == 0x49)
				{
					returnVal = KeyTypes::I;
				}
				else if (pRawInput->data.keyboard.VKey == 0x4A)
				{
					returnVal = KeyTypes::J;
				}
				else if (pRawInput->data.keyboard.VKey == 0x4B)
				{
					returnVal = KeyTypes::K;
				}
				else if (pRawInput->data.keyboard.VKey == 0x4C)
				{
					returnVal = KeyTypes::L;
				}
				else if (pRawInput->data.keyboard.VKey == 0x4D)
				{
					returnVal = KeyTypes::M;
				}
				else if (pRawInput->data.keyboard.VKey == 0x4E)
				{
					returnVal = KeyTypes::N;
				}
				else if (pRawInput->data.keyboard.VKey == 0x4F)
				{
					returnVal = KeyTypes::O;
				}
				else if (pRawInput->data.keyboard.VKey == 0x50)
				{
					returnVal = KeyTypes::P;
				}
				else if (pRawInput->data.keyboard.VKey == 0x51)
				{
					returnVal = KeyTypes::Q;
				}
				else if (pRawInput->data.keyboard.VKey == 0x52)
				{
					returnVal = KeyTypes::R;
				}
				else if (pRawInput->data.keyboard.VKey == 0x53)
				{
					returnVal = KeyTypes::S;
				}
				else if (pRawInput->data.keyboard.VKey == 0x54)
				{
					returnVal = KeyTypes::T;
				}
				else if (pRawInput->data.keyboard.VKey == 0x55)
				{
					returnVal = KeyTypes::U;
				}
				else if (pRawInput->data.keyboard.VKey == 0x56)
				{
					returnVal = KeyTypes::V;
				}
				else if (pRawInput->data.keyboard.VKey == 0x57)
				{
					returnVal = KeyTypes::W;
				}
				else if (pRawInput->data.keyboard.VKey == 0x58)
				{
					returnVal = KeyTypes::X;
				}
				else if (pRawInput->data.keyboard.VKey == 0x59)
				{
					returnVal = KeyTypes::Y;
				}
				else if (pRawInput->data.keyboard.VKey == 0x5A)
				{
					returnVal = KeyTypes::Z;
				}
				else if (pRawInput->data.keyboard.VKey == VK_LEFT)
				{
					returnVal = KeyTypes::ArrowLeft;
				}
				else if (pRawInput->data.keyboard.VKey == VK_RIGHT)
				{
					returnVal = KeyTypes::ArrowRight;
				}
				else if (pRawInput->data.keyboard.VKey == VK_UP)
				{
					returnVal = KeyTypes::ArrowUp;
				}
				else if (pRawInput->data.keyboard.VKey == VK_DOWN)
				{
					returnVal = KeyTypes::ArrowDown;
				}
				else if (pRawInput->data.keyboard.VKey == VK_ESCAPE)
				{
					returnVal = KeyTypes::Escape;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD0)
				{
					returnVal = KeyTypes::Num0;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD1)
				{
					returnVal = KeyTypes::Num1;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD2)
				{
					returnVal = KeyTypes::Num2;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD3)
				{
					returnVal = KeyTypes::Num3;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD4)
				{
					returnVal = KeyTypes::Num4;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD5)
				{
					returnVal = KeyTypes::Num5;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD6)
				{
					returnVal = KeyTypes::Num6;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD7)
				{
					returnVal = KeyTypes::Num7;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD8)
				{
					returnVal = KeyTypes::Num8;
				}
				else if (pRawInput->data.keyboard.VKey == VK_NUMPAD9)
				{
					returnVal = KeyTypes::Num9;
				}
				else if (pRawInput->data.keyboard.VKey == VK_SPACE)
				{
					returnVal = KeyTypes::Space;
				}
				else
					returnVal = KeyTypes::Null;
			}

		} // namespace Input
	} // namespace GLib
}