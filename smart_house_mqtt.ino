/* ********************************************** 
Code generate by generator_for_esp32_mqtt
https://github.com/Semelion/generator_for_esp32_mqtt
developed by Semelion 
********************************************** */

#define	lamp1	23
#define	lamp2	22
#define	lamp3	21
#define	button1	19
#define	button2	18
#define	button3	5

void setup(){
	pinmode(lamp1, OUTPUT);
	pinmode(lamp2, OUTPUT);
	pinmode(lamp3, OUTPUT);
	pinmode(button1, INPUT);
	pinmode(button2, INPUT);
	pinmode(button3, INPUT);
}
