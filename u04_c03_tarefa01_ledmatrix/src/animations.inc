/* ANIMAÇÃO DA TECLA A */
{{{0/* (todos LEDs desligados) */}}},

/* ANIMAÇÃO DA TECLA B */
{/* (todos LEDs ligados na cor azul com 100% de intensidade) */
.palette = {{0x00, 0x00, 0xff}}},

/* ANIMAÇÃO DA TECLA C */
{/* (todos LEDs ligados na cor vermelha com 80% de intensidade) */
.palette = {{0xff*.8f, 0x00, 0x00}}},

/* ANIMAÇÃO DA TECLA D */
{/* (todos LEDs ligados na cor verde com 50% de intensidade) */
.palette = {{0x00, 0xff*.5f, 0x00}}},

/* ANIMAÇÃO DA TECLA # */
{/* (todos LEDs ligados na cor branca com 20% de intensidade) */
.palette = {{0xff*.2f, 0xff*.2f, 0xff*.2f}}},

/* ANIMAÇÃO 01 */
{.palette = {
	{0x00, 0x00,       0x00},
	{0x00, 0xff * .1f, 0x00},
	{0xff, 0x00,       0x00}
},

.frames = {
	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 1, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 0, 0,
	 1, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 1, 0,
	 1, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 1, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 1, 1, 1, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 1, 1, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 1, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 2,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 2, 0, 2,
	 0, 0, 0, 2, 0},

	{0, 0, 0, 0, 0,
	 1, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 1, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 2, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 1, 1, 1, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 1, 1, 1,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 1, 1, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 1, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0, /* 30 */
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},
}
},

/* ANIMAÇÃO 02 */
{.palette = {
	{0x00, 0x00, 0x00},
	{0xbf, 0x55, 0x00}
},

.frames = {
	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0,
	 0, 1, 0, 1, 0,
	 0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0,
	 1, 0, 1, 0, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0},

	{0, 1, 0, 0, 0,
	 0, 0, 1, 0, 1,
	 0, 1, 0, 1, 0,
	 1, 0, 1, 0, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0, /* 05 */
	 0, 1, 0, 1, 0,
	 0, 0, 1, 0, 0,
	 0, 1, 0, 1, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 1, 1, 1, 0,
	 0, 1, 0, 1, 0,
	 0, 1, 1, 1, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 1, 1, 1, 0,
	 1, 1, 0, 1, 1,
	 0, 1, 1, 1, 0,
	 0, 0, 1, 0, 0},

	{1, 0, 0, 0, 1,
	 0, 1, 1, 1, 0,
	 0, 1, 0, 1, 0,
	 0, 1, 1, 1, 0,
	 1, 0, 0, 0, 1},

	{0, 0, 1, 0, 0,
	 0, 1, 1, 1, 0,
	 1, 1, 0, 1, 1,
	 0, 1, 1, 1, 0,
	 0, 0, 1, 0, 0},

	{1, 1, 1, 1, 1, /* 10 */
	 1, 0, 0, 0, 1,
	 1, 0, 1, 0, 1,
	 1, 0, 0, 0, 1,
	 1, 1, 1, 1, 1},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 1, 0,
	 0, 1, 0, 0, 0,
	 0, 0, 0, 1, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 1, 0, 1, 0, 0,
	 0, 0, 0, 0, 1},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0, /* 15 */
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 1, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 1, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0, /* 20 */
	 0, 0, 0, 1, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 0, 1, 0, 0,
	 0, 1, 1, 1, 0,
	 0, 0, 1, 0, 0,
	 0, 0, 1, 0, 0},

	{0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0,
	 1, 1, 0, 1, 1,
	 0, 0, 1, 0, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 1, 1, 1, 0,
	 0, 0, 1, 0, 0,
	 0, 1, 1, 1, 0,
	 0, 0, 1, 0, 0},

	{0, 0, 0, 0, 0, /* 25 */
	 0, 1, 1, 1, 0,
	 0, 1, 0, 1, 0,
	 0, 1, 1, 1, 0,
	 0, 0, 0, 0, 0},

	{0, 0, 1, 0, 0,
	 0, 0, 1, 0, 0,
	 1, 1, 1, 1, 1,
	 0, 0, 1, 0, 0,
	 0, 0, 1, 0, 0},

	{0, 1, 0, 0, 0,
	 0, 1, 0, 0, 1,
	 0, 1, 1, 1, 0,
	 1, 0, 0, 1, 0,
	 0, 0, 0, 1, 0},

	{1, 0, 0, 0, 1,
	 0, 1, 0, 1, 0,
	 0, 0, 1, 0, 0,
	 0, 1, 0, 1, 0,
	 1, 0, 0, 0, 1},

	{0, 0, 0, 1, 0,
	 1, 1, 1, 1, 0,
	 0, 1, 1, 1, 0,
	 0, 1, 1, 1, 1,
	 0, 1, 0, 0, 0},

	{0, 0, 1, 0, 0, /* 30 */
	 0, 0, 0, 0, 0,
	 1, 0, 0, 0, 1,
	 0, 0, 0, 0, 0,
	 0, 0, 1, 0, 0}}
},

/* vim: set ft=c: */
