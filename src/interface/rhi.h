#pragma once
namespace Aura {
    class RHI {
        public:
            virtual void initialize() = 0;
            virtual void render_test() = 0;
        private:

    };
}