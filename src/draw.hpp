void create_window();
void poll_events();
void render_frame();
void exit_emu();
bool draw_byte(uint8_t x,uint8_t y,uint8_t byte);
void clear_display();
uint8_t get_random(uint8_t num);
bool is_key_pressed(uint8_t keynum);