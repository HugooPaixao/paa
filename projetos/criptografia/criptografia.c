#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

static uint8_t mul2[256], mul3[256], mul9[256], mulB[256], mulD[256], mulE[256];

void init_mul_tables() {
    for(int i = 0; i < 256; i++) {
        uint8_t c = 0, a = i;
        for(int b = 0; b < 8; b++) {
            if(a & 0x80) c ^= 0x1B;
            a <<= 1;
        }
        mul2[i] = c ^ i;
        mul3[i] = mul2[i] ^ i;
        mul9[i] = mul2[mul2[mul2[i]]] ^ i;
        mulB[i] = mul2[mul9[i]] ^ i;
        mulD[i] = mul2[mul9[i] ^ i] ^ i;
        mulE[i] = mul2[mulD[i]] ^ i;
    }
}

// tabela de substituicao para o AES
static const uint8_t SBOX[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// tabela de inversao para o AES
static const uint8_t INV_SBOX[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

// constantes para cada rodada do AES
static const uint8_t RCON[11] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

static inline uint8_t mul_gf_fast(uint8_t a, uint8_t b) {
    if(b == 0x02) return mul2[a];
    if(b == 0x03) return mul3[a];
    if(b == 0x09) return mul9[a];
    if(b == 0x0B) return mulB[a];
    if(b == 0x0D) return mulD[a];
    if(b == 0x0E) return mulE[a];

    uint8_t c = 0;
    for(int i=0;i<8;i++) {
        if(b&1) c ^= a;
        uint8_t h = a&0x80;
        a <<= 1;
        if(h) a ^= 0x1B;
        b >>= 1;
    }
    return c;
}

void subtrairBytes(uint8_t s[4][4]) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            s[i][j]=SBOX[s[i][j]];
}

void inv_subtrairBytes(uint8_t s[4][4]) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            s[i][j]=INV_SBOX[s[i][j]];
}

void shiftLinhas(uint8_t s[4][4]) {
    uint8_t t;
    t = s[1][0]; s[1][0] = s[1][1]; s[1][1] = s[1][2]; s[1][2] = s[1][3]; s[1][3] = t;
    t = s[2][0]; s[2][0] = s[2][2]; s[2][2] = t; t = s[2][1]; s[2][1] = s[2][3]; s[2][3] = t;
    t = s[3][3]; s[3][3] = s[3][2]; s[3][2] = s[3][1]; s[3][1] = s[3][0]; s[3][0] = t;
}

void inv_shiftLinhas(uint8_t s[4][4]) {
    uint8_t t;
    t = s[1][3]; s[1][3] = s[1][2]; s[1][2] = s[1][1]; s[1][1] = s[1][0]; s[1][0] = t;
    t = s[2][0]; s[2][0] = s[2][2]; s[2][2] = t; t = s[2][1]; s[2][1] = s[2][3]; s[2][3] = t;
    t = s[3][0]; s[3][0] = s[3][1]; s[3][1] = s[3][2]; s[3][2] = s[3][3]; s[3][3] = t;
}

void embaralharColunas(uint8_t s[4][4]) {
    for(int j=0;j<4;j++) {
        uint8_t a=s[0][j], b=s[1][j], c=s[2][j], d=s[3][j];
        s[0][j] = mul_gf_fast(0x02,a)^mul_gf_fast(0x03,b)^c^d;
        s[1][j] = a^mul_gf_fast(0x02,b)^mul_gf_fast(0x03,c)^d;
        s[2][j] = a^b^mul_gf_fast(0x02,c)^mul_gf_fast(0x03,d);
        s[3][j] = mul_gf_fast(0x03,a)^b^c^mul_gf_fast(0x02,d);
    }
}

void inv_embaralharColunas(uint8_t s[4][4]) {
    for(int j=0;j<4;j++) {
        uint8_t a=s[0][j], b=s[1][j], c=s[2][j], d=s[3][j];
        s[0][j]=mul_gf_fast(0x0E,a)^mul_gf_fast(0x0B,b)^mul_gf_fast(0x0D,c)^mul_gf_fast(0x09,d);
        s[1][j]=mul_gf_fast(0x09,a)^mul_gf_fast(0x0E,b)^mul_gf_fast(0x0B,c)^mul_gf_fast(0x0D,d);
        s[2][j]=mul_gf_fast(0x0D,a)^mul_gf_fast(0x09,b)^mul_gf_fast(0x0E,c)^mul_gf_fast(0x0B,d);
        s[3][j]=mul_gf_fast(0x0B,a)^mul_gf_fast(0x0D,b)^mul_gf_fast(0x09,c)^mul_gf_fast(0x0E,d);
    }
}

void adicionarChave(uint8_t s[4][4], uint8_t k[4][4]) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            s[i][j] ^= k[i][j];
}

void rotacionarPalavra(uint8_t *w) {
    uint8_t t=w[0];
    w[0] = w[1]; w[1] = w[2]; w[2] = w[3]; w[3] = t;
}

void substituirPalavra(uint8_t *w) {
    for(int i=0;i<4;i++)
        w[i]=SBOX[w[i]];
}

void expansaoChave(uint8_t *key, uint8_t rk[11][4][4]) {
    uint8_t w[44][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            w[i][j] = key[4*i+j];

    for(int i=4;i<44;i++) {
        uint8_t t[4];
        memcpy(t,w[i-1],4);

        if(i%4 == 0) {
            rotacionarPalavra(t);
            substituirPalavra(t);
            t[0] ^= RCON[i/4];
        }

        for(int j=0;j<4;j++)
            w[i][j] = w[i-4][j] ^ t[j];
    }

    for(int r=0;r<11;r++)
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                rk[r][j][i] = w[4*r+i][j];
}

void bytesParaMatriz(uint8_t *dado, uint8_t s[4][4]) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            s[j][i] = dado[i*4+j];
}

void matrizParaByte(uint8_t s[4][4], uint8_t *dado) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            dado[i*4+j] = s[j][i];
}

void encriptarBloco(uint8_t *p, uint8_t *c, uint8_t rk[11][4][4]) {
    uint8_t s[4][4];
    bytesParaMatriz(p,s);
    adicionarChave(s,rk[0]);

    for(int r=1;r<10;r++) {
        subtrairBytes(s);
        shiftLinhas(s);
        embaralharColunas(s);
        adicionarChave(s,rk[r]);
    }

    subtrairBytes(s);
    shiftLinhas(s);
    adicionarChave(s,rk[10]);
    matrizParaByte(s,c);
}

void decriptarBloco(uint8_t *c, uint8_t *p, uint8_t rk[11][4][4]) {
    uint8_t s[4][4];
    bytesParaMatriz(c,s);
    adicionarChave(s,rk[10]);

    for(int r=9;r>=1;r--) {
        inv_shiftLinhas(s);
        inv_subtrairBytes(s);
        adicionarChave(s,rk[r]);
        inv_embaralharColunas(s);
    }

    inv_shiftLinhas(s);
    inv_subtrairBytes(s);
    adicionarChave(s,rk[0]);
    matrizParaByte(s,p);
}

void encriptarAES(uint8_t *p, size_t tam, uint8_t *key, uint8_t *c, size_t *ctam) {
    uint8_t rk[11][4][4];
    expansaoChave(key,rk);
    size_t ptam = ((tam+15)/16)*16;
    uint8_t *pd = calloc(ptam,1);
    memcpy(pd,p,tam);

    for(size_t i=0; i<ptam; i+=16)
        encriptarBloco(pd+i, c+i, rk);

    *ctam = ptam;
    free(pd);
}

void decripitarAES(uint8_t *c, size_t tam, uint8_t *key, uint8_t *p) {
    uint8_t rk[11][4][4];
    expansaoChave(key,rk);

    for(size_t i=0; i<tam; i+=16)
        decriptarBloco(c+i, p+i, rk);
}

typedef struct {
    uint8_t *dado;
    size_t tam;
} BigInt;

void inicializarBigInt(BigInt *a) {
    a->dado = NULL; a->tam = 0;
}

void liberaBigInt(BigInt *a) {
    if(a->dado) free(a->dado);
    a->dado = NULL;
    a->tam = 0;
}

void BigIntZero(BigInt *a) {
    liberaBigInt(a);
    a->dado = calloc(1,1);
    a->tam = 1;
}

void hexParaBigInt(BigInt *a, const char *hex) {
    BigIntZero(a);
    size_t hextam = strlen(hex);
    size_t bytetam = (hextam+1)/2;
    uint8_t *bytes = malloc(bytetam);
    size_t bi = 0;

    if(hextam % 2) {
        sscanf(hex, "%1hhx", &bytes[0]); bi++; hex++; hextam--;
    }

    for(size_t i=0; i<hextam/2; i++) {
        sscanf(hex+2*i, "%2hhx", &bytes[bi+i]);
    }

    bytetam = bi + hextam/2;
    liberaBigInt(a);
    a->dado = malloc(bytetam);
    a->tam = bytetam;

    for(size_t i=0; i<bytetam; i++)
        a->dado[i] = bytes[bytetam-1-i];

    free(bytes);
}

void BigIntParaHex(BigInt *a, char *hex, size_t tamEsperadoHex) {
    if(a->tam == 0) {
        for(size_t i=0;i<tamEsperadoHex;i++)
            hex[i]='0';
        hex[tamEsperadoHex]=0;
        return;
    }

    size_t bytetam = a->tam;
    uint8_t *be = malloc(bytetam);

    for(size_t i=0;i<bytetam;i++)
        be[i] = a->dado[bytetam-1-i];

    size_t iniciar = 0;
    while(iniciar < bytetam && be[iniciar] == 0)
        iniciar++;

    if(iniciar == bytetam)
        iniciar = bytetam-1;

    size_t tamEfetivo = bytetam - iniciar;
    char *tmp = malloc(tamEfetivo*2+1);

    for(size_t i=0;i<tamEfetivo;i++)
        sprintf(tmp+2*i, "%02X", be[iniciar+i]);
    free(be);
    size_t tmp_tam = strlen(tmp);

    if(tmp_tam >= tamEsperadoHex) {
        strcpy(hex, tmp + tmp_tam - tamEsperadoHex);
    }
    else {
        size_t pad = tamEsperadoHex - tmp_tam;
        memset(hex, '0', pad);
        strcpy(hex+pad, tmp);
    }

    hex[tamEsperadoHex] = 0;
    free(tmp);
}

int compBigInt(BigInt *a, BigInt *b) {
    if(a->tam != b->tam) return a->tam > b->tam ? 1 : -1;

    for(size_t i=a->tam; i-- > 0; )
        if(a->dado[i] != b->dado[i])
            return a->dado[i] > b->dado[i] ? 1 : -1;
    return 0;
}

void BigIntSub(BigInt *r, BigInt *a, BigInt *b) {
    uint8_t *res = calloc(a->tam, 1);
    int16_t borrow = 0;
    for(size_t i=0; i<a->tam; i++) {
        int16_t av = a->dado[i];
        int16_t bv = (i < b->tam) ? b->dado[i] : 0;
        int16_t diff = av - bv - borrow;

        if(diff < 0) { diff += 256; borrow = 1; } else borrow = 0;
        res[i] = diff & 0xFF;
    }

    size_t newtam = a->tam;
    while(newtam > 1 && res[newtam-1] == 0)
        newtam--;

    liberaBigInt(r);
    r->dado = res;
    r->tam = newtam;
}

void BigIntMult(BigInt *r, BigInt *a, BigInt *b) {
    size_t atam = a->tam, btam = b->tam;
    uint8_t *res = calloc(atam+btam, 1);

    for(size_t i=0; i<atam; i++) {
        uint16_t vaiUm = 0;
        uint8_t ai = a->dado[i];

        for(size_t j=0; j<btam || vaiUm; j++) {
            uint16_t bj = (j<btam) ? b->dado[j] : 0;
            uint16_t produto = res[i+j] + (uint16_t)ai * bj + vaiUm;
            res[i+j] = produto & 0xFF;
            vaiUm = produto >> 8;
        }
    }

    size_t newtam = atam+btam;
    while(newtam>1 && res[newtam-1] == 0)
        newtam--;

    liberaBigInt(r);
    r->dado = res;
    r->tam = newtam;
}

void BigIntMod(BigInt *r, BigInt *a, BigInt *m) {
    if(compBigInt(a, m) < 0) {
        liberaBigInt(r);
        r->dado = malloc(a->tam);
        memcpy(r->dado, a->dado, a->tam);
        r->tam = a->tam;
        return;
    }

    BigInt rem, div;
    inicializarBigInt(&rem); inicializarBigInt(&div);
    rem.dado = malloc(a->tam);
    memcpy(rem.dado, a->dado, a->tam);
    rem.tam = a->tam;
    size_t shift = (a->tam - m->tam) * 8;

    if(a->dado[a->tam-1] < m->dado[m->tam-1]) {
        if(shift >= 8) shift -= 8;
        else shift = 0;
    }
    size_t div_bytes = m->tam + (shift+7)/8;
    div.dado = calloc(div_bytes,1);

    for(size_t i=0; i<m->tam; i++) {
        size_t pos = i + shift/8;
        uint16_t val = m->dado[i];
        div.dado[pos] |= (val << (shift%8)) & 0xFF;
        if((shift%8) && pos+1 < div_bytes)
            div.dado[pos+1] = val >> (8 - (shift%8));
    }

    div.tam = div_bytes;
    while(div.tam>1 && div.dado[div.tam-1] == 0)
        div.tam--;

    while(compBigInt(&rem, m) >= 0) {
        if(compBigInt(&div, &rem) <= 0) {
            BigIntSub(&rem, &rem, &div);
        }
        else {
            uint16_t vaiUm = 0;
            for(size_t i=div.tam; i-- > 0;) {
                uint16_t atual = (vaiUm << 8) + div.dado[i];
                div.dado[i] = atual >> 1;
                vaiUm = atual & 1;
            }
            while(div.tam>1 && div.dado[div.tam-1] == 0) div.tam--;
        }
    }

    liberaBigInt(r);
    r->dado = rem.dado; rem.dado = NULL;
    r->tam = rem.tam;
    liberaBigInt(&rem);
    liberaBigInt(&div);
}

void BigIntMultMod(BigInt *r, BigInt *a, BigInt *b, BigInt *m) {
    BigInt produto;
    inicializarBigInt(&produto);
    BigIntMult(&produto, a, b);
    BigIntMod(r, &produto, m);
    liberaBigInt(&produto);
}

void BigIntPotMod(BigInt *r, BigInt *base, BigInt *exp, BigInt *mod) {
    BigInt res, b, e;

    inicializarBigInt(&res);
    inicializarBigInt(&b);
    inicializarBigInt(&e);
    BigIntZero(&res);

    res.dado[0] = 1;
    BigIntMod(&b, base, mod);
    e.dado = malloc(exp->tam);
    memcpy(e.dado, exp->dado, exp->tam);
    e.tam = exp->tam;

    while(!(e.tam == 1 && e.dado[0] == 0)) {
        if(e.dado[0] & 1) {
            BigIntMultMod(&res, &res, &b, mod);
        }

        BigIntMultMod(&b, &b, &b, mod);
        uint16_t vaiUm = 0;

        for(size_t i = e.tam; i-- > 0;) {
            uint16_t atual = (vaiUm << 8) + e.dado[i];
            e.dado[i] = atual >> 1;
            vaiUm = atual & 1;
        }

        while(e.tam>1 && e.dado[e.tam-1] == 0)
            e.tam--;
    }

    liberaBigInt(r);
    r->dado = res.dado; res.dado = NULL;
    r->tam = res.tam;
    liberaBigInt(&res);
    liberaBigInt(&b);
    liberaBigInt(&e);
}

size_t hexaParaByte(const char *hex, uint8_t *bytes) {
    size_t tam = strlen(hex);
    size_t bi = 0;

    if(tam % 2) {
        sscanf(hex, "%1hhx", &bytes[0]); bi++; hex++; tam--;
    }

    for(size_t i=0; i<tam/2; i++) {
        sscanf(hex+2*i, "%2hhx", &bytes[bi+i]);
    }
    return bi + tam/2;
}

void byteParaHex(const uint8_t *bytes, size_t tam, char *hex) {
    for(size_t i=0; i<tam; i++)
        sprintf(hex+2*i, "%02X", bytes[i]);
    hex[2*tam] = '\0';
    fflush(stdout);
}

void removerQuebra(char *s) {
    size_t l = strlen(s);
    while(l>0 && (s[l-1]=='\n' || s[l-1]=='\r'))
        s[--l]=0;
}

int main(int argc, char *argv[]) {
    init_mul_tables();

    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    char line[16384];
    fgets(line, sizeof(line), input);
    removerQuebra(line);
    int n = atoi(line);

    uint8_t chaveAES[16] = {0};

    for(int i=0; i<n; i++) {
        fgets(line, sizeof(line), input);
        removerQuebra(line);
        char op[16];
        sscanf(line, "%s", op);

        if(strcmp(op, "dh") == 0) {
            char a_hex[4096], bHex[4096], gHex[8192], pHex[8192];
            sscanf(line, "%*s %s %s %s %s", a_hex, bHex, gHex, pHex);

            BigInt ba, bb, bg, bp, bab, bs;

            inicializarBigInt(&ba);
            inicializarBigInt(&bb);
            inicializarBigInt(&bg);
            inicializarBigInt(&bp);
            inicializarBigInt(&bab);
            inicializarBigInt(&bs);
            hexParaBigInt(&ba, a_hex);
            hexParaBigInt(&bb, bHex);
            hexParaBigInt(&bg, gHex);
            hexParaBigInt(&bp, pHex);
            BigIntMult(&bab, &ba, &bb);
            BigIntPotMod(&bs, &bg, &bab, &bp);

            char sHex[2048];
            BigIntParaHex(&bs, sHex, 32);
            fprintf(output, "s=%s\n", sHex);

            memset(chaveAES, 0, 16);
            size_t copiarTam = (bs.tam < 16) ? bs.tam : 16;

            for(size_t j=0; j<copiarTam; j++) {
                chaveAES[15-j] = bs.dado[j];
            }

            liberaBigInt(&ba);
            liberaBigInt(&bb);
            liberaBigInt(&bg);
            liberaBigInt(&bp);
            liberaBigInt(&bab);
            liberaBigInt(&bs);

        }
        else if(strcmp(op, "d") == 0) {
            char cHex[16384];
            sscanf(line, "%*s %s", cHex);

            uint8_t bytesC[8192], bytesM[8192];
            size_t cl = hexaParaByte(cHex, bytesC);
            size_t al = ((cl+15)/16)*16;

            for(size_t j=cl; j<al; j++)
                bytesC[j] = 0;

            decripitarAES(bytesC, al, chaveAES, bytesM);

            char mHex[8192];
            byteParaHex(bytesM, al, mHex);
            fprintf(output, "m=%s\n", mHex);

        }
        else if(strcmp(op, "e") == 0) {
            char mHex[32768];
            sscanf(line, "%*s %s", mHex);

            int hl = strlen(mHex);
            int al = ((hl+31)/32)*32;
            char alinharM[32768];
            strcpy(alinharM, mHex);

            for(int j=hl; j<al; j++)
                alinharM[j] = '0';

            alinharM[al] = '\0';

            uint8_t bytesM[16384], bytesC[16384];
            size_t ml = hexaParaByte(alinharM, bytesM);
            size_t cl;
            encriptarAES(bytesM, ml, chaveAES, bytesC, &cl);

            char cHex[32768];
            byteParaHex(bytesC, cl, cHex);
            fprintf(output, "c=%s\n", cHex);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}