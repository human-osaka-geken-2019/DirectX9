#ifndef GRAPHIC_H
#define GRAPHIC_H

//! 名称はヒューマンアカデミー大阪校の伝統
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

void RotateZ(CUSTOMVERTEX* vertex, float rad) {

	float vertexX[4];
	float vertexY[4];

	vertexX[0] = vertex[0].x;
	vertexX[1] = vertex[1].x;
	vertexX[2] = vertex[2].x;
	vertexX[3] = vertex[3].x;

	vertexY[0] = vertex[0].y;
	vertexY[1] = vertex[1].y;
	vertexY[2] = vertex[2].y;
	vertexY[3] = vertex[3].y;

	float centerX = (vertexX[1] - vertexX[0]) * 0.5f + vertexX[0];
	float centerY = (vertexY[3] - vertexY[0]) * 0.5f + vertexY[0];

	for (int i = 0; i < 4; i++) {

		vertexX[i] -= centerX;
		vertexY[i] -= centerY;

		float buf = vertexX[i];

		vertexX[i] = (vertexX[i] * cos(-rad)) - (vertexY[i] * sin(-rad));
		vertexY[i] = (vertexY[i] * cos(-rad)) + (buf * sin(-rad));

		vertexX[i] += centerX;
		vertexY[i] += centerY;

		vertex[i].x = vertexX[i];
		vertex[i].y = vertexY[i];
	}

}

#endif

