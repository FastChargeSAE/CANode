#include "interrupt.h"

	/*	TODO
	 * PVD interrupt monitora la tensione di alimentazione
	 * per proteggere il processore proverei a spegnerlo via software
	 * e per notificare questa situazione manderei un pacchetto via CAN
	 * con la tensione critica che è stata rivelata e il nome della scheda*/
void PVD_IRQHandler(void){

}


/*gli interrupt di tutti gli ADC sono generici, viene alzato sempre questo interrupt
 * bisogna controllare quale flag è stato asserita per capire cosa è successo
 *TODO OVR, EOC, AWD*/
void ADC_IRQHandler(void){
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET){
		//do something alla fine della conversione

		ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
	}
	else if(ADC_GetFlagStatus(ADC1,ADC_FLAG_AWD)==SET){
		//un regular channel è andato oltre le soglie

		ADC_ClearFlag (ADC1,ADC_FLAG_AWD);
	}
	else if(ADC_GetFlagStatus(ADC1,ADC_FLAG_OVR)==SET){
		//l'ADC1 è andato in OVERRUN!! DMA disabilitate, tocca farle ripartire pag 400 RM0090

		ADC_ClearFlag (ADC1,ADC_FLAG_OVR);
	}

}

// TODO
void DMA2_Stream0_IRQHandler() {

}


//TODO
void CAN2_TX_IRQHandler(void){

}

//TODO
void CAN2_RX0_IRQHandler(void){
	extern uint32_t potVCU;

	CanRxMsg RxMessage;

	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);


	if(RxMessage.StdId == May12)
	{
		potVCU = RxMessage.Data[0];
		potVCU<<=8;
		potVCU |= RxMessage.Data[1];
		potVCU<<=8;
		potVCU |= RxMessage.Data[2];
		potVCU<<=8;
		potVCU |= RxMessage.Data[3];
		potVCU<<=8;
		if (potVCU > 0xff0a)
			GPIO_init(GPIOB,LSfet, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_2MHz, GPIO_PuPd_UP);
		else
			GPIO_init(GPIOB,LSfet, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_2MHz, GPIO_PuPd_DOWN);



		//		switch (bus_state){
		//		//disabilita timer per l'invio di pacchetti
		//		case 0:
		//			if(bus_state) TIM_Cmd (TIM2, DISABLE);
		//			break;
		//
		//			//abilita il timer per l'invio di pacchetti
		//		case 1:
		//			if (!bus_state)	TIM_Cmd (TIM2, ENABLE);
		//			break;
		//		}
	}
	else CAN_Manage_Rx(RxMessage);
}
