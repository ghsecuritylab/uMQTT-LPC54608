/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "azure_umqtt_c/mqtt_client.h"
#include "azure_umqtt_c/mqtt_message.h"
#include "MQTTEcho.h"
#include "lwip/dns.h"
#if MQTT_USE_TLS
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/tlsio_mbedtls.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "certs/certs.h"
#endif
#include "azure_c_shared_utility/socketio.h"
#include "azure_c_shared_utility/xio.h"

typedef struct MQTT_CLIENT_INFO_TAG
{
	char* hostName;
	char* username;
	char* password;
	char* x509_client_cert;
	char* private_key;
	char* certificate;
};
sys_thread_t xMqttThread;

static const char* TOPIC_NAME_A = "LPC54608/TEST";
static const char* TOPIC_NAME_B = "WICED/TEST";
static const char* APP_NAME_A = "This is the app msg A.";

static uint16_t PACKET_ID_VALUE = 11;
static bool g_continue = true;

static void OnRecvCallback(MQTT_MESSAGE_HANDLE  msgHandle, void* context)
{
	static uint32_t messageCount = 0;
	MQTT_CLIENT_HANDLE mqttHandle = *((MQTT_CLIENT_HANDLE *)context);
	const APP_PAYLOAD* messagePayload = mqttmessage_getApplicationMsg(msgHandle);
	PRINTF("Incoming Msg: Packet Id: %d\r\nQOS: %d\r\nTopic Name: %s\r\nIs Retained: %s\r\nIs Duplicate: %s\r\nApp Msg: ",
			mqttmessage_getPacketId(msgHandle),
			mqttmessage_getQosType(msgHandle),
			mqttmessage_getTopicName(msgHandle),
			mqttmessage_getIsRetained(msgHandle) ? "true" : "false",
		    mqttmessage_getIsDuplicateMsg(msgHandle) ? "true" : "false"
	);
	for (size_t index = 0; index < messagePayload->length; index++)
	{
		PRINTF("%c", messagePayload->message[index]);
	}
#if MQTT_USE_TLS
	MQTT_MESSAGE_HANDLE msg = mqttmessage_create(PACKET_ID_VALUE++, TOPIC_NAME_B, DELIVER_EXACTLY_ONCE, (const uint8_t*)APP_NAME_A, strlen(APP_NAME_A));
#else
	MQTT_MESSAGE_HANDLE msg = mqttmessage_create(PACKET_ID_VALUE++, TOPIC_NAME_A, DELIVER_EXACTLY_ONCE, (const uint8_t*)APP_NAME_A, strlen(APP_NAME_A));
#endif
	if (msg == NULL)
	{
		PRINTF("%d: mqttmessage_create failed\r\n", __LINE__);
		g_continue = false;
	}
	else
	{
		if (mqtt_client_publish(mqttHandle, msg))
		{
			PRINTF("%d: mqtt_client_publish failed\r\n", __LINE__);
			g_continue = false;
		}
		mqttmessage_destroy(msg);
	}
	PRINTF("\r\nmessage count %d\r\n", ++messageCount);
}

static void OnOperationComplete(MQTT_CLIENT_HANDLE handle, MQTT_CLIENT_EVENT_RESULT actionResult, const void* msgInfo, void* callbackCtx)
{
    (void)msgInfo;
    (void)callbackCtx;
    switch (actionResult)
    {
        case MQTT_CLIENT_ON_CONNACK:
        {
            PRINTF("ConnAck function called\r\n");

            SUBSCRIBE_PAYLOAD subscribe;
            subscribe.subscribeTopic = TOPIC_NAME_A;
            subscribe.qosReturn = DELIVER_EXACTLY_ONCE;
            if (mqtt_client_subscribe(handle, PACKET_ID_VALUE++, &subscribe, sizeof(subscribe) / sizeof(SUBSCRIBE_PAYLOAD)) != 0)
            {
                PRINTF("%d: mqtt_client_subscribe failed\r\n", __LINE__);
                g_continue = false;
            }
            PRINTF("mqtt_client_subscribe success\r\n");
            break;
        }
        case MQTT_CLIENT_ON_SUBSCRIBE_ACK:
        {
        	PRINTF("mqtt subscribe ack received\n");
            MQTT_MESSAGE_HANDLE msg = mqttmessage_create(PACKET_ID_VALUE++, TOPIC_NAME_A, DELIVER_EXACTLY_ONCE, (const uint8_t*)APP_NAME_A, strlen(APP_NAME_A));
            if (msg == NULL)
            {
                PRINTF("%d: mqttmessage_create failed\r\n", __LINE__);
                g_continue = false;
            }
            else
            {
                if (mqtt_client_publish(handle, msg))
                {
                    PRINTF("%d: mqtt_client_publish failed\r\n", __LINE__);
                    g_continue = false;
                }
                mqttmessage_destroy(msg);
            }
            // Now send a message that will get
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_ACK:
        {
//        	PRINTF("mqtt client publish ack received\n");
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_RECV:
        {
//        	PRINTF("MQTT_CLIENT_ON_PUBLISH_RECV\n");
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_REL:
        {
//        	PRINTF("MQTT_CLIENT_ON_PUBLISH_REL\n");
            break;
        }
        case MQTT_CLIENT_ON_PUBLISH_COMP:
        {
//        	PRINTF("MQTT_CLIENT_ON_PUBLISH_COMP\n");
            // Done so send disconnect
            //mqtt_client_disconnect(handle, NULL, NULL);
            break;
        }
        case MQTT_CLIENT_ON_DISCONNECT:
            g_continue = false;
            break;
        case MQTT_CLIENT_ON_UNSUBSCRIBE_ACK:
        {
        	PRINTF("MQTT_CLIENT_ON_UNSUBSCRIBE_ACK\n");
            break;
        }
        case MQTT_CLIENT_ON_PING_RESPONSE:
        	break;
        default:
        {
            PRINTF("unexpected value received for enumeration (%d)\n", (int)actionResult);
        }
    }
}

static void OnErrorComplete(MQTT_CLIENT_HANDLE handle, MQTT_CLIENT_EVENT_ERROR error, void* callbackCtx)
{
    (void)callbackCtx;
    (void)handle;
    switch (error)
    {
    case MQTT_CLIENT_CONNECTION_ERROR:
    case MQTT_CLIENT_PARSE_ERROR:
    case MQTT_CLIENT_MEMORY_ERROR:
    case MQTT_CLIENT_COMMUNICATION_ERROR:
    case MQTT_CLIENT_NO_PING_RESPONSE:
    case MQTT_CLIENT_UNKNOWN_ERROR:
        g_continue = false;
        break;
    }
}

void prvMQTTEchoTask(void *pvParameters)
{
    /* connect to m2m.eclipse.org, subscribe to a topic, send and receive messages regularly every 1 sec */
    MQTT_CLIENT_HANDLE mqttHandle;
    uint32_t taskNotifyValue;
    if (xTaskNotifyWait(0x01, 0x01, &taskNotifyValue, portMAX_DELAY) == pdFALSE)
    {
        vTaskDelete(NULL);
    }
    PRINTF("START MQTT Task\r\n");
    mqttHandle = mqtt_client_init(OnRecvCallback, OnOperationComplete, (void*)(&mqttHandle), OnErrorComplete, (void*)(&mqttHandle));
    if (mqttHandle == NULL)
    	vTaskDelete(NULL);
    MQTT_CLIENT_OPTIONS mqttOptions = { 0 };
    mqttOptions.clientId = "LPC54608";
    mqttOptions.willMessage = NULL;
    mqttOptions.username = NULL;
    mqttOptions.password = NULL;
    mqttOptions.keepAliveInterval = 10;
    mqttOptions.useCleanSession = true;
    mqttOptions.qualityOfServiceValue = DELIVER_EXACTLY_ONCE;
#if !(MQTT_USE_TLS)
    SOCKETIO_CONFIG socketConfig = {"iot.eclipse.org", 1883, NULL};
    XIO_HANDLE mqttXioHandle = xio_create(socketio_get_interface_description(), &socketConfig);
#else
    TLSIO_CONFIG tlsConfig;
    memset(&tlsConfig, 0, sizeof(TLSIO_CONFIG));
    tlsConfig.hostname = "192.168.1.19";
    tlsConfig.port = 8883;
    XIO_HANDLE mqttXioHandle = xio_create(tlsio_mbedtls_get_interface_description(), (void *)&tlsConfig);
    xio_setoption(mqttXioHandle, OPTION_TRUSTED_CERT, (void*)&rootCa);
    xio_setoption(mqttXioHandle, OPTION_X509_ECC_KEY, (void*)&privateKey);
    xio_setoption(mqttXioHandle, OPTION_X509_ECC_CERT, (void*)&clientCert);
#endif // !defined(MQTT_USE_TLS)
    if (mqtt_client_connect(mqttHandle, mqttXioHandle, &mqttOptions) != 0)
    {
    	PRINTF("mqtt_client_connect failed\n");
    }
    else
    {
    	PRINTF("mqtt_client_connect success\n");
    	while (g_continue)
    	{
    		mqtt_client_dowork(mqttHandle);
    	}
    }
    vTaskDelete(NULL);
}


void vStartMQTTTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
    BaseType_t x = 0L;
    
    xTaskCreate(prvMQTTEchoTask,	/* The function that implements the task. */
                "MQTTEcho0",			/* Just a text name for the task to aid debugging. */
                usTaskStackSize,	/* The stack size is defined in FreeRTOSIPConfig.h. */
                (void *)x,		/* The task parameter, not used in this case. */
                uxTaskPriority,		/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
                NULL);				/* The task handle is not used. */
}
/*-----------------------------------------------------------*/

