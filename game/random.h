#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	Random();
	float frandom();
	int irandom(int n);
	float frandom (float lo, float hi);
	int irandom (int lo, int hi);
	void reset();
private:
	double n;
	double x;

};

#endif // RANDOM_H
