#include "calc.hpp"

#ifndef PC
APP_NAME("Simple 3D Render")
APP_DESCRIPTION("Cool")
APP_AUTHOR("JH1SC")
APP_VERSION("1.0.0")
#endif

#define White 0xffff // white
#define Black 0x0000 // black

// Taylor Expansion to approximate Sin(Angle)
float sin(float x, int n)
{
	float sinx = 0;
	float term = x;
	for (int i = 1; i <= n; i++)
	{
		sinx += term;
		term *= -x * x / ((2 * i) * (2 * i + 1));
	}
	return sinx;
}

// Taylor Expansion to approximate Cos(Angle)
float cos(float x, int n)
{
	float cosx = 1;
	float term = 1;
	for (int i = 1; i <= n; i++)
	{
		term *= -x * x / ((2 * i - 1) * (2 * i));
		cosx += term;
	}
	return cosx;
}

void circle(int x, int y, int r, uint16_t c)
{
	for (int ix = -r; ix <= r; ix++)
		for (int iy = -r; iy <= r; iy++)
			if (ix * ix + iy * iy < r * r) // a circle are all points where x^2+y^2=r^2 is true.
				// setPixel takes care to check if the dot is on the screen.
				setPixel(x + ix, y + iy, c);
}

// The acutal main
void main2()
{

	fillScreen(color(0, 64, 0));
	println("Works BB");
	// Vertices Vertices
	float Vertices[8][3] = {
		//    x    y    z
		{0.1, 0.1, 0.1},
		{0.1, 0.1, 1},
		{0.1, 1, 0.1},
		{0.1, 1, 1},
		{1, 0.1, 0.1},
		{1, 0.1, 1},
		{1, 1, 0.1},
		{1, 1, 1}};

	int Faces[13][3] = {
		{0, 1, 3},
		{0, 3, 2},
		{4, 6, 7},
		{4, 7, 5},
		{0, 4, 5},
		{0, 5, 1},
		{2, 3, 7},
		{2, 7, 6},
		{0, 2, 6},
		{0, 6, 4},
		{1, 5, 7},
		{1, 7, 3}
	};

	//int vAL = (sizeof(Vertices) / sizeof(*Vertices)); //  Vertex Array Length (Rows)
	// int fAL = (sizeof(Faces) / sizeof(*Faces));		  //  Face Array Length (Rows)

	int Scale = 80;
	int FOV = 40;
	float cosTheta = cos(0.1, 20);
	float sinTheta = sin(0.1, 20);
	LCD_Refresh();
	println("(-) to exit.");
	while (true)
	{
		fillScreen(Black);
		uint32_t key1, key2;
		getKey(&key1, &key2);
		if (testKey(key1, key2, KEY_NEGATIVE))
		{
			break;
		}

		if (testKey(key1, key2, KEY_UP))
		{
			for (int i = 0; i < 8; i++)
			{
				float y1 = ((float)Vertices[i][1]);
				float z1 = ((float)Vertices[i][2]);
				Vertices[i][1] = ((float)y1) * ((float)cosTheta) - ((float)z1) * ((float)sinTheta);
				Vertices[i][2] = ((float)y1) * ((float)sinTheta) + ((float)z1) * ((float)cosTheta);
			}
		}

		if (testKey(key1, key2, KEY_DOWN))
		{
			for (int i = 0; i < 8; i++)
			{
				float y1 = ((float)Vertices[i][1]);
				float z1 = ((float)Vertices[i][2]);
				Vertices[i][1] = ((float)y1) * ((float)cosTheta) + ((float)z1) * ((float)sinTheta);
				Vertices[i][2] = ((float)z1) * ((float)cosTheta) - ((float)y1) * ((float)sinTheta);
			}
		}

		if (testKey(key1, key2, KEY_LEFT))
		{
			for (int i = 0; i < 8; i++)
			{
				float x1 = ((float)Vertices[i][0]);
				float z1 = ((float)Vertices[i][2]);
				Vertices[i][0] = ((float)x1) * ((float)cosTheta) + ((float)z1) * ((float)sinTheta);
				Vertices[i][2] = ((float)z1) * ((float)cosTheta) - ((float)x1) * ((float)sinTheta);
			}
		}

		if (testKey(key1, key2, KEY_RIGHT))
		{
			for (int i = 0; i < 8; i++)
			{
				float x1 = ((float)Vertices[i][0]);
				float z1 = ((float)Vertices[i][2]);
				Vertices[i][0] = ((float)x1) * ((float)cosTheta) - ((float)z1) * ((float)sinTheta);
				Vertices[i][2] = ((float)z1) * ((float)cosTheta) + ((float)x1) * ((float)sinTheta);
			}
		}
		for (int i = 0; i < 13; i++)
		{
			float x1 = Vertices[(Faces[i][0])][0];
			float y1 = Vertices[(Faces[i][0])][1];
			float z1 = Vertices[(Faces[i][0])][2];

			float x2 = Vertices[(Faces[i][1])][0];
			float y2 = Vertices[(Faces[i][1])][1];
			float z2 = Vertices[(Faces[i][1])][2];

			float x3 = Vertices[(Faces[i][2])][0];
			float y3 = Vertices[(Faces[i][2])][1];
			float z3 = Vertices[(Faces[i][2])][2];

			int nx1 = (((float)Vertices[i][0]) * (FOV / (FOV + ((float)z1))) * Scale) + 128;
			int ny1 = (((float)Vertices[i][1]) * (FOV / (FOV + ((float)z1))) * Scale) + 128;

			int nx2 = (((float)Vertices[i][0]) * (FOV / (FOV + ((float)z2))) * Scale) + 128;
			int ny2 = (((float)Vertices[i][1]) * (FOV / (FOV + ((float)z2))) * Scale) + 128;

			int nx3 = (((float)Vertices[i][0]) * (FOV / (FOV + ((float)z3))) * Scale) + 128;
			int ny3 = (((float)Vertices[i][1]) * (FOV / (FOV + ((float)z3))) * Scale) + 128;


			triangle(nx1,ny1,nx2,ny2,nx3,ny3,White,White);
		}
		LCD_Refresh();
	}
}
