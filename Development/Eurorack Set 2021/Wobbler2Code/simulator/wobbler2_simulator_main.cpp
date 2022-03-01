#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

class MyStream : public sf::SoundStream
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

                for (k = 0; k < numKeys; k++)
                {
                        if (vol[k] <= 0) continue;

                        increment = PI2 * pow(2.0, (k + 3 + 12 * octave) / 12.0) * baseAFreq / sampleRate;

                        for (c = 0; c < audioBufSize; c += numChannel)
                        {
                                if (waveShapeType == ShapeMode::sine)
                                {
                                        s = m_samples[c] + sin(phase[k])*vol[k];
                                }
                                else if (waveShapeType == ShapeMode::triangle)
                                {
                                        double t2 = fmodf(phase[k], PI2) / (PI2);
                                        int t3 = floor(t2 + 0.5f);
                                        double t4 = fabs(t2 - t3) * 4 - 1;
                                        s = m_samples[c] + t4 * vol[k];
                                }
                                else if (waveShapeType == ShapeMode::saw)
                                {
                                        double t2 = phase[k] / PI2;
                                        int t3 = floor(t2 + 0.5f);
                                        double t4 = (t2 - t3) * 2;
                                        s = m_samples[c] + t4 * vol[k];
                                }
                                else if (waveShapeType == ShapeMode::square)
                                {
                                        double t2 = fmodf(phase[k], PI2) / (PI2);
                                        double t3 = t2 > 0.5 ? 1 : -1;
                                        s = m_samples[c] + t3 * vol[k];
                                }
                                else if (waveShapeType == ShapeMode::whiteNoise)
                                {
                                        s = rand() % 65536 - 32768;
                                }
                                
                                if (s > 32767) s = 32767;
                                else if (s < -32768) s = -32768;

                                m_samples[c] = s;
                                m_samples[c + 1] = s;

                                phase[k] += increment;

                                if (vol[k] < MAXVOL)
                                {
                                        vol[k] -= DECAY;
                                        if (vol[k] <= 0)
                                        {
                                                vol[k] = 0;
                                                break;
                                        }
                                }
                        }
                        phase[k] = fmod(phase[k], PI2);
                }

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

    MyStream stream;
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


        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}