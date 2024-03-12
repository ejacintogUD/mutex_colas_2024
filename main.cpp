/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "MemoryPool.h"
#include "Mutex.h"
#include "PinNameAliases.h"
#include "Queue.h"
#include "ThisThread.h"
#include "Thread.h"
#include "UnbufferedSerial.h"
#include "mbed.h"
#include "mbed_rtx_conf.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

// Prototipos de la funcion 
void func_tx_1(void);
void func_tx_2(void);
void func_productora(void);



//Tareas y funciones del sistema operativo
Thread th_func_tx_1(osPriorityNormal, OS_STACK_SIZE);
Thread th_func_tx_2(osPriorityNormal, OS_STACK_SIZE);
Thread th_func_productora(osPriorityNormal, OS_STACK_SIZE);

Mutex m_serie;

typedef struct
{

 float  a; 

} cola_data;

MemoryPool<cola_data, 16> mpool;
Queue <cola_data, 16> queue;



// Variables globales 
char i=0;
char men[30];

// puertos 
UnbufferedSerial pc(USBTX, USBRX);


DigitalOut led(LED1);
int main()
{
    th_func_tx_1.start(func_tx_1);
    th_func_tx_2.start(func_tx_2);
    th_func_productora.start(func_productora);

    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}

void func_tx_1 (void)
{
    const char men_t1[30] = ("Tarea 1 \n\r");
    while(true)
    {
        m_serie.lock();
        i++;
        sprintf(men, "Tarea 1 %03u \n\r", i);
        pc.write(men, sizeof(men));
        m_serie.unlock();
        ThisThread::sleep_for(1s);
    }
}


void func_tx_2 (void)
{
    char men_i[10];    
    const char men_t2[30] = ("Tarea 2 \n\r");
    while(true)
    {
        m_serie.lock();
        i++;
        sprintf(men, "Tarea 2 %03u \n\r", i);
        pc.write(men, sizeof(men));
        m_serie.unlock();
        ThisThread::sleep_for(1s);
    }
}

void func_productora(void)
{
    while(true)
    {
        cola_data  *queue = mpool.try_alloc(); // pedir espacio en memoria
        queue->a = (rand()*255);

    }
}

