
typedef string FileName<>;

enum ChubbyEvent {
  NOP = 0,
  LOCK_CHANGED = 0x100,
  CONTENT_MODIFIED = 0x200,
};

struct EventContent {
  ChubbyEvent event;
  FileName fname;
};

program event_handler {
  version handler_v1 {
      void event_callback(EventContent) = 77;
  } = 1;
} = 0x40048086;
