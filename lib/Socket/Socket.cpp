#include "Socket.hpp"

Socket::Socket(std::string wifi_ssid, std::string wifi_password) {
	Socket::isConnected = false;

	// Connect to WiFi
	WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
	wl_status_t status = (wl_status_t)WiFi.waitForConnectResult(5000);
	if (status != WL_CONNECTED) {
		Serial.println("Failed to connect to WiFi");  // TODO: handle correctly
	}

	// Initialize Websocket
	esp_websocket_client_config_t ws_cfg = {
		.host = "192.168.0.11",	 // TODO: Use true values
		.port = 8080,
		.transport = WEBSOCKET_TRANSPORT_OVER_TCP};
	this->ws = esp_websocket_client_init(&ws_cfg);
	if (this->ws == NULL) {
		Serial.println(
			"Failed to initialize websocket client");  // TODO: handle correctly
	}

	// Register Data Handler
	auto result = esp_websocket_register_events(this->ws, WEBSOCKET_EVENT_DATA,
												Socket::onData, nullptr);
	if (result != ESP_OK) {
		Serial.println(
			"Failed to register data handler");	 // TODO: handle correctly
	}

	// Register Connect Handler
	result = esp_websocket_register_events(this->ws, WEBSOCKET_EVENT_CONNECTED,
										   Socket::onConnect, nullptr);
	if (result != ESP_OK) {
		Serial.println(
			"Failed to register connect handler");	// TODO: handle correctly
	}

	// Register Disconnect Handler
	result = esp_websocket_register_events(
		this->ws, WEBSOCKET_EVENT_DISCONNECTED, Socket::onDisconnect, nullptr);
	if (result != ESP_OK) {
		Serial.println(
			"Failed to register disconnect handler");  // TODO: handle correctly
	}

	// Register Closed Handler
	result = esp_websocket_register_events(this->ws, WEBSOCKET_EVENT_CLOSED,
										   Socket::onClosed, nullptr);
	if (result != ESP_OK) {
		Serial.println(
			"Failed to register closed handler");  // TODO: handle correctly
	}

	// Register Error Handler
	result = esp_websocket_register_events(this->ws, WEBSOCKET_EVENT_CLOSED,
										   Socket::onError, nullptr);
	if (result != ESP_OK) {
		Serial.println(
			"Failed to register error handler");  // TODO: handle correctly
	}

	esp_websocket_client_start(this->ws);
}

Socket::~Socket() {
	esp_websocket_client_stop(this->ws);
	esp_websocket_client_destroy(this->ws);
}

std::string getColor(uint8_t color) {
	switch (color) {
		case P_BLUE:
			return "BLUE";
		case P_GREEN:
			return "GREEN";
		case P_RED:
			return "RED";
		case P_YELLOW:
			return "YELLOW";
	}
	return "";
}

void Socket::send(Player *players[4]) {
	StaticJsonDocument<256> doc;

	for (int j = 0; j < 4; j++) {
		auto positions = players[j]->getPositions();
		for (int i = 0; i < 4; i++) {
			doc[getColor(players[j]->getColor())].add(positions[i]);
		}
	}

	std::string serialized;
	serializeJson(doc, serialized);

	int result = esp_websocket_client_send_text(
		this->ws, serialized.c_str(), serialized.length(), portMAX_DELAY);
	if (result == -1) {
		Serial.println("Failed to send message");  // TODO: handle correctly
	}
}

void Socket::connect() {
	while (!Socket::isConnected) {
		delay(10);
	}
}

void Socket::onData(void *event_handler_arg, esp_event_base_t event_base,
					int32_t event_id, void *event_data) {
	esp_websocket_event_data_t *event =
		(esp_websocket_event_data_t *)event_data;

	if (event->data_len > 0) {	// TODO
		Serial.println("Received message:");
		Serial.write(event->data_ptr, event->data_len);
		Serial.println();
	}
}

void Socket::onConnect(void *event_handler_arg, esp_event_base_t event_base,
					   int32_t event_id, void *event_data) {
	esp_websocket_event_data_t *event =
		(esp_websocket_event_data_t *)event_data;
	Socket::isConnected = true;

	Serial.println("Websocket connected");
}

void Socket::onDisconnect(void *event_handler_arg, esp_event_base_t event_base,
						  int32_t event_id,
						  void *event_data) {  // TODO: implement
	Serial.println("Websocket disconnected");
}

void Socket::onClosed(void *event_handler_arg, esp_event_base_t event_base,
					  int32_t event_id, void *event_data) {	 // TODO: implement
	Serial.println("Websocket closed");
}

void Socket::onError(void *event_handler_arg, esp_event_base_t event_base,
					 int32_t event_id, void *event_data) {	// TODO: implement
	Serial.println("Websocket error");
}