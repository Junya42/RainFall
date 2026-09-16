int main(int argc, char **argv)
{
    FILE *f;
    char buf[132];
    int  n;

    f = fopen("/home/user/end/.pass", "r");
    memset(buf, 0, sizeof(buf));
    if (!f || argc != 2)
        return -1;

    fread(buf, 1, 66, f);
    buf[65] = 0;
    n = atoi(argv[1]);
    buf[n] = 0;
    fread(buf + 66, 1, 65, f);
    fclose(f);

    if (strcmp(buf, argv[1]) == 0)
        execl("/bin/sh", "sh", NULL);
    else
        puts(buf + 66);
    return 0;
}