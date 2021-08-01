//OTHON AUGUSTO - 2020003021
//PROGRAMAÇÃO EMBARCADAS
//PROF. OTAVIO GOMES
//Projeto Final

#include "keypad.h"
#include "lcd.h"
#include "bits.h"
#include "config.h"
#include "ssd.h"
#include "io.h"
#include "pwm.h"

#include <pic18f4520.h>

#define ON 0x0F
#define L0 0x80
#define L1 0xC0
#define L2 0x90
#define L3 0xD0
#define CLR 0x01

#define DISP0 PIN_A2
#define DISP1 PIN_A3
#define DISP2 PIN_A4
#define DISP3 PIN_A5

char i;

//pqueno delay

void delay() {
    char i, j;
    for (i = 0; i < 255; i++) {
        for (j = 0; j < 255; j++);
    }
}

//efeito shift

void shift(int t, int op) {
    for (i = 0; i < 5; i++) {
        delay();
        lcdCommand(0x1C); //lcd desliza pra direita
    }
    if (op == 1) return;
    for (i = 0; i < 5; i++) {
        delay();
        lcdCommand(0x18); //lcd desliza pra esquerda
    }
    delay();
}

//introducao trab

void introducao() {
    lcdCommand(CLR);
    char text[12] = "Bem Vindo ao";
    char text2[16] = "Othon CIA Voo";
    lcdCommand(L1 | (0));
    for (i = 0; i < 12; i++) {
        lcdData(text[i]);
    }
    lcdCommand(L2 | (0));
    for (i = 0; i < 13; i++) {
        lcdData(text2[i]);
    }

    delay();
    shift(400, 0);
}

//opcoes

void opcoes() {
    lcdCommand(CLR);
    char text[15] = "Escolha um voo:";
    char text1[10] = "1 - Disney ";
    char text2[9] = "2 - Paris";
    char text3[12] = "3 - Hogwarts";
    lcdCommand(L0 | (0));
    for (i = 0; i < 15; i++) {
        lcdData(text[i]);
    }
    lcdCommand(L1 | (0));
    for (i = 0; i < 10; i++) {
        lcdData(text1[i]);
    }
    lcdCommand(L2 | (0));
    for (i = 0; i < 9; i++) {
        lcdData(text2[i]);
    }
    lcdCommand(L3 | (0));
    for (i = 0; i < 12; i++) {
        lcdData(text3[i]);
    }
}

//msg de inicio de viajem

void iniciandoViajem() {
    lcdCommand(CLR);

    char text[11] = "Viajando...";
    lcdCommand(L0 | (0));
    for (i = 0; i < 10; i++) {
        lcdData(text[i]);
    }
}

//msg de fim de viajem

void terminandoViajem() {
    lcdCommand(CLR);

    char text[11] = "Voce Chegou ";
    lcdCommand(L0 | (0));
    for (i = 0; i < 11; i++) {
        lcdData(text[i]);
    }
    delay();
    delay();
    delay();
    delay();
}

//cronometro x segundos, setar hz para 8

void cronometro(int temp) {
    for (int aux = 0; aux <= temp * 100 + 1; aux++) {
        int cont = aux / 100;
        if (cont >= 10) {
            ssdDigit(cont / 10, 2);
            ssdDigit(cont % 10, 3);
        } else {
            ssdDigit(0, 2);
            ssdDigit(cont, 3);
        }
        ssdUpdate();
        for (float tempo = 0; tempo < 100; tempo++); // +- 10ms
    }
    //desligando display 7-seg
    digitalWrite(DISP0, 0);
    digitalWrite(DISP1, 0);
    digitalWrite(DISP2, 0);
    digitalWrite(DISP3, 0);
}

//leds ligam de cima para baixo

void verso() {
    delay();
    unsigned char k, x;
    x = 0b10000001;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x << 1;
    }
}

//leds ligam de baixo para cima

void inverso() {
    delay();
    unsigned char k, x;
    x = 0b10000000;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x >> 1;
    }
}

//liga leds, cooler e mostra msg de pane

void paradaEmergencial() {
    lcdCommand(CLR);

    pwmSet(99);
    pwmFrequency(1000);

    char text[16] = "POUSO EMERGENCIAL";
    lcdCommand(L0 | (0));
    for (i = 0; i < 15; i++) {
        lcdData(text[i]);
    }
    shift(400, 0);
    verso();
    inverso();
    verso();
    inverso();

    pwmSet(0);
    pwmFrequency(0);

    lcdCommand(CLR);
}

void main(void) {
    unsigned int tecla = 16;

    //inicializando libs
    ssdInit();
    lcdInit();
    kpInit();
    pwmInit();
    lcdCommand(ON);

    introducao();

    opcoes();

    for (;;) {
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 3)) { //1
                lcdCommand(CLR);
                iniciandoViajem();
                //30 min
                cronometro(30);
                terminandoViajem();
            }
            if (bitTst(tecla, 7)) { //2
                lcdCommand(CLR);
                iniciandoViajem();
                //10 min
                cronometro(10);
                paradaEmergencial();
                iniciandoViajem();
                cronometro(10);
                terminandoViajem();
            }
            if (bitTst(tecla, 11)) { //3
                lcdCommand(CLR);
                char text[16] = "Voce Nao e Bruxo";
                lcdCommand(L0 | (0));
                for (i = 0; i < 16; i++) {
                    lcdData(text[i]);
                }
            }
            if (bitTst(tecla, 8)) { //#
                lcdCommand(CLR);
                introducao();
                opcoes();
            }
            if (bitTst(tecla, 0)) { //*
                lcdCommand(CLR);
                char fim[16] = "Obrigado Prof";
                lcdCommand(L0 | (0));
                for (i = 0; i < 16; i++) {
                    lcdData(fim[i]);
                }
            }

        }

    }
}






