#pragma once
class bgTile
{
public:
	int x, y;
	char filename;
	void set_x(int);
	void set_y(int);
	void set_filename(char);
	int get_x();
	int get_y();
	char get_filename();
	bgTile();
	~bgTile();
};

void bgTile::set_x(int a) {
	x = a;
}

void bgTile::set_y(int b) {
	y = b;
}

void bgTile::set_filename(char file) {
	filename = file;
}

int bgTile::get_x() {
	return x;
}

int bgTile::get_y() {
	return y;
}

char bgTile::get_filename() {
	return filename;
}
