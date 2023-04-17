

#include "_game.hpp"

class Game;

class Socket {
   public:
	Socket(std::string wifi_ssid, std::string wifi_password);
	~Socket();

	/**
	 * @brief Send the current game state to the server
	 *
	 * @param game The current game state
	 */
	void send(Player *players[4]);

	/**
	 * @brief Waits until the socket is connected
	 */
	void connect();

   private:
	esp_websocket_client_handle_t ws;
	static inline volatile bool isConnected;

	static inline void onData(void *event_handler_arg,
							  esp_event_base_t event_base, int32_t event_id,
							  void *event_data);
	static inline void onConnect(void *event_handler_arg,
								 esp_event_base_t event_base, int32_t event_id,
								 void *event_data);
	static inline void onDisconnect(void *event_handler_arg,
									esp_event_base_t event_base,
									int32_t event_id, void *event_data);
	static inline void onClosed(void *event_handler_arg,
								esp_event_base_t event_base, int32_t event_id,
								void *event_data);
	static inline void onError(void *event_handler_arg,
							   esp_event_base_t event_base, int32_t event_id,
							   void *event_data);
};