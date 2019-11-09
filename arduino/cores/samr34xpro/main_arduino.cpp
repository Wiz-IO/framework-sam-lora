
extern void initVariant();
extern void setup();
extern void loop();

extern "C" int main(void)
{
    initVariant();
    setup();
    while (1)
    {
        loop();
    }
}