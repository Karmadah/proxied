#include "csgo.hpp"

namespace csgo {
	player_t* local_player = nullptr;
	bool sending_packet = NULL;
	namespace fonts {
		unsigned long watermark_font;
		unsigned long name_font;
	}
}