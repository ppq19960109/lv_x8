#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MQTTAsync.h"

#define ROKI_MQTT_HOST "tcp://mqtt.myroki.com:1883"
#define ROKI_MQTT_USERNAME "admin"
#define ROKI_MQTT_PASSWORD "jnkj2014"

#define ADDRESS "tcp://mqtt.eclipseprojects.io:1883"
#define CLIENTID "ExampleClientSub"
#define TOPIC "MQTT Examples"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

static MQTTAsync client;
int disc_finished = 0;
int subscribed = 0;
int finished = 0;

void onConnect(void *context, MQTTAsync_successData *response);
void onConnectFailure(void *context, MQTTAsync_failureData *response);

void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	if (cause)
		printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		finished = 1;
	}
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: %.*s\n", message->payloadlen, (char *)message->payload);
	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}

void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Disconnect failed, rc %d\n", response->code);
	disc_finished = 1;
}

void onDisconnect(void *context, MQTTAsync_successData *response)
{
	printf("Successful disconnection\n");
	disc_finished = 1;
}

void onSubscribe(void *context, MQTTAsync_successData *response)
{
	printf("Subscribe succeeded\n");
	subscribed = 1;
}

void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Subscribe failed, rc %d\n", response->code);
	finished = 1;
}

void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
	printf("Connect failed, rc %d\n", response->code);
	finished = 1;
}

void onConnect(void *context, MQTTAsync_successData *response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc;

	printf("Successful connection\n");

	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
		   "Press Q<Enter> to quit\n\n",
		   TOPIC, CLIENTID, QOS);
	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;
	if ((rc = MQTTAsync_subscribe(client, TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code %d\n", rc);
		finished = 1;
	}
}


int roki_mqtt_init()
{
	int rc;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;

	if ((rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to create client, return code %d\n", rc);
		rc = EXIT_FAILURE;
		goto exit;
	}

	if ((rc = MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to set callbacks, return code %d\n", rc);
		rc = EXIT_FAILURE;
		goto exit;
	}

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	conn_opts.automaticReconnect = 1; // 设置非零，断开自动重连，默认是0
	conn_opts.minRetryInterval = 3;	  // 单位秒，重连间隔次数，每次重新连接失败时，重试间隔都会加倍，直到最大间隔
	conn_opts.maxRetryInterval = 30;  // 单位秒，最大重连尝试间隔
	conn_opts.username = ROKI_MQTT_USERNAME;
	conn_opts.password = ROKI_MQTT_PASSWORD;

	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		rc = EXIT_FAILURE;
	}

exit:
	return rc;
}

void roki_mqtt_deinit()
{
	int rc;
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;

	disc_opts.onSuccess = onDisconnect;
	disc_opts.onFailure = onDisconnectFailure;
	if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		rc = EXIT_FAILURE;
		goto destroy_exit;
	}
	while (!disc_finished)
	{
		usleep(10000L);
	}

destroy_exit:
	MQTTAsync_destroy(&client);
}
