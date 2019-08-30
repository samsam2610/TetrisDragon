#include "Piece.h"

int Piece::IDs = 0;

void Piece::rotate() {
	vector<vector<int>> newShape{ shape };

    for (int i = 0; i < shape.size(); i++) {
		for (int j = 0; j < shape[i].size(); j++) {
			newShape[j][shape.size() - i - 1] = shape[i][j];
		}
    }

    countRotation += 1;
	setShape(newShape);

}

void Piece::moveRight() {
	x++;
}

void Piece::moveLeft() {
	x--;
}

void Piece::moveDown() {
	y++;
}

bool Piece::hasBody() const {
	return !shape.empty();
}
