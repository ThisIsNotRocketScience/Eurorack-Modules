#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

int sampleRate = 44100;
int numChannel = 2;
int audioBufSize = 2048;

class AudioStream : public sf::SoundStream
{
public:

        void SetBufSize(int sz, int numChannel, int sampleRate)
        {
                m_samples.resize(sz, 0);
                m_currentSample = 0;
                initialize(numChannel, sampleRate);
        }

private:

        virtual bool onGetData(Chunk& data)
        {
                int len = m_samples.size();
                for (int i = 0; i < len; ++i)
                {
                    m_samples[i] = 0;
                }

                data.samples = &m_samples[m_currentSample];

                int k = 0;
                int c = 0;
                float s = 0;
                float increment = 0;

                data.sampleCount = audioBufSize;
                m_currentSample = 0;

                return true;
        }

        virtual void onSeek(sf::Time timeOffset)
        {
                m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
        }

        std::vector<sf::Int16> m_samples;
        std::size_t m_currentSample;
};



int main() {

    AudioStream stream;
    stream.SetBufSize(audioBufSize, numChannel, sampleRate);
    stream.play();


    sf::RenderWindow window(sf::VideoMode(1280, 720), "TiNRS Wobbler2 Simulator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Button("hah!");
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}