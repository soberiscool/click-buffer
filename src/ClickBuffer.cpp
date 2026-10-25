#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GameManager.hpp>

using namespace geode::prelude;

class ClickBuffer {
public:
    static ClickBuffer& get() {
        static ClickBuffer instance;
        return instance;
    }

    void onClick(bool isDown) {
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        
        int bufferFrames = Mod::get()->getSettingValue<int64_t>("buffer-frames");
        if (bufferFrames <= 0) return;

        std::string mode = Mod::get()->getSettingValue<std::string>("buffer-mode");
        
        // Store click with timestamp
        ClickData click;
        click.isDown = isDown;
        click.frame = GameManager::get()->getGameFrame();
        click.processed = false;
        
        m_clickBuffer.push_back(click);
        
        // Clean old clicks (older than buffer frames)
        int maxAge = bufferFrames + 2;
        auto it = m_clickBuffer.begin();
        while (it != m_clickBuffer.end()) {
            if (GameManager::get()->getGameFrame() - it->frame > maxAge) {
                it = m_clickBuffer.erase(it);
            } else {
                ++it;
            }
        }
    }

    bool checkBufferedClick(bool isDown) {
        if (!Mod::get()->getSettingValue<bool>("enabled")) return false;
        
        int bufferFrames = Mod::get()->getSettingValue<int64_t>("buffer-frames");
        if (bufferFrames <= 0) return false;

        std::string mode = Mod::get()->getSettingValue<std::string>("buffer-mode");
        int currentFrame = GameManager::get()->getGameFrame();
        
        // Check if we have a buffered click that matches
        for (auto it = m_clickBuffer.begin(); it != m_clickBuffer.end(); ++it) {
            if (it->processed) continue;
            if (it->isDown != isDown) continue;
            
            int frameDiff = currentFrame - it->frame;
            
            bool valid = false;
            if (mode == "before" && frameDiff >= 0 && frameDiff <= bufferFrames) {
                valid = true; // Click happened before, within buffer
            } else if (mode == "after" && frameDiff <= 0 && frameDiff >= -bufferFrames) {
                valid = true; // Click will happen after, within buffer
            } else if (mode == "both" && frameDiff >= -bufferFrames && frameDiff <= bufferFrames) {
                valid = true; // Click within buffer window
            }
            
            if (valid) {
                it->processed = true;
                return true;
            }
        }
        
        return false;
    }

    void clear() {
        m_clickBuffer.clear();
    }

private:
    struct ClickData {
        bool isDown;
        int frame;
        bool processed;
    };
    
    std::vector<ClickData> m_clickBuffer;
};

// Hook PlayerObject::pushButton to intercept clicks
class $modify(PlayerObject) {
    bool pushButton(int button) {
        // Button 0 = jump/click
        if (button == 0) {
            ClickBuffer::get().onClick(true);
            
            // Check if we have a buffered click that should register
            if (ClickBuffer::get().checkBufferedClick(true)) {
                return PlayerObject::pushButton(button);
            }
            
            // If no buffer match, still allow normal click
            return PlayerObject::pushButton(button);
        }
        return PlayerObject::pushButton(button);
    }

    bool releaseButton(int button) {
        if (button == 0) {
            ClickBuffer::get().onClick(false);
            
            if (ClickBuffer::get().checkBufferedClick(false)) {
                return PlayerObject::releaseButton(button);
            }
            
            return PlayerObject::releaseButton(button);
        }
        return PlayerObject::releaseButton(button);
    }

    void resetObject() {
        ClickBuffer::get().clear();
        PlayerObject::resetObject();
    }
};

// Also hook GameManager for frame counting if needed
class $modify(GameManager) {
    int m_gameFrame = 0;
    
    int getGameFrame() {
        return ++m_gameFrame;
    }
    
    void resetGameFrame() {
        m_gameFrame = 0;
    }
};

$on_mod(Loaded) {
    log::info("Click Buffer loaded! Buffer frames: {}", 
        Mod::get()->getSettingValue<int64_t>("buffer-frames"));
}
