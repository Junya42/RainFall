#include <unistd.h>
#include <cstring>

class N {
public:
	int nb;
	int (N::*func)(N &);
	char annotation[100];

	N(int val) : nb(val)
	{
		this->func = &N::operator+;
	}
	int operator+(N &right)
	{
		return this->nb + right.nb;
	}
	int operator-(N &right)
	{
		return this->nb - right.nb;
	}
	void setAnnotation(char *str)
	{
		memcpy(this->annotation, str, strlen(str));
	}
};

int		main(int argc, char **argv)
{
	if (argc <= 1)
        _exit(1);

    N *n5 = new N(5);
    N *n6 = new N(6);

    n5->setAnnotation(argv[1]);
    *n6 + *n5;
    return 0;
}