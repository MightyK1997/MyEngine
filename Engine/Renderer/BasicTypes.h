#pragma once

#include <stdint.h>

namespace Engine
{
	namespace Renderer
	{
		typedef void(*KeyStateChange)(unsigned int i_VKeyID, bool i_bDown);

		struct Point2D
		{
			float			x;
			float			y;
		};

		struct UV
		{
			float			u, v;
		};

		struct RGBA
		{
			uint8_t			r, g, b, a;
		};

		namespace Sprites
		{
			typedef struct _SpriteEdges
			{
				float			Left;
				float			Top;
				float			Right;
				float			Bottom;
			} SpriteEdges;

			typedef struct _SpriteUVs
			{
				UV				TopLeft;
				UV				TopRight;
				UV				BottomLeft;
				UV				BottomRight;
			} SpriteUVs;
		} // namespace Sprites
	}
	enum KeyTypes
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ArrowUp, ArrowDown, ArrowLeft, ArrowRight, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,
		Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key0, BackSpace, Enter, LeftShift, RightShift, Escape, Null, Space
	};
}