#pragma once

#if defined( CAPTUREMANAGER_WHITEBOX )
#define CAPTUREMANAGER_BOX public:
#else
#define CAPTUREMANAGER_BOX private:
#endif