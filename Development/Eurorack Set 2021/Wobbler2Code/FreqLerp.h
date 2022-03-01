#define FREQLERPLEN 17
#define ADJ(y,x) ((unsigned long)(x*(1.0f + y/4.0f)))

unsigned long FreqLerp[FREQLERPLEN] =
{
		ADJ(0,0x29f16),
		ADJ(1,0x3903f),
		ADJ(2,0x4d812),
		ADJ(3,0x695b2),
		ADJ(4,0x8f377),
		ADJ(5,0xc2aeb),
		ADJ(6,0x108a49),
		ADJ(7,0x167be7),
		ADJ(8,0x1e9053),
		ADJ(9,0x298c0c),
		ADJ(10,0x387a2f),
		ADJ(11,0x4cc5d4),
		ADJ(12,0x685c8f),
		ADJ(13,0x8ddd55),
		ADJ(14,0xc0d836),
		ADJ(15,0x1062501),
		ADJ(16,0x1645912)
};

