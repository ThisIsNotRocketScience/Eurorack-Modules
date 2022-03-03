#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "../../Wobbler2Drum.h"
#include "../Wobbler2.cpp"
#include "../Pendulum.cpp"
#include "../../EurorackShared/Math.c"
#include "../../EurorackShared/ImGuiThings.c"

Wobbler2Drum TheDrum;

int sampleRate = 44100;
int numChannel = 2;
int audioBufSize = 2048;
bool Trigger = false;
float fFreq = 0.0f;
float fMod = 0.0f;
float fShape = 0.0f;
float fPhase = 0.0f;
float fNormalAmt = 0.0f;
float fPhasedAmt = 0.0f;


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
    virtual bool onGetData(Chunk &data)
    {

        uint16_t uFreq = (int)(fFreq * 0xffff);
        uint16_t uMod = (int)(fMod * 0xffff);
        uint16_t uPhase = (int)(fPhase * 0xffff);
        uint16_t uPhasedAmt = (int)(fPhasedAmt * 0xffff);
        uint16_t uShape = (int)(fShape * 0xffff);
        uint16_t uNormalAmt = (int)(fNormalAmt * 0xffff);
        TheDrum.Freq = uFreq;
        TheDrum.Shape = uShape;
        TheDrum.Mod = uMod;
        TheDrum.Phase = uPhase;
        
        if (Trigger)
        {
            TheDrum.Trigger();
            Trigger = false;
        }

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
        for (int i = 0; i < len; i += 2)
        {
            int L = 0, R = 0;
            TheDrum.Get(L, R);
            m_samples[i] = L;
            m_samples[i + 1] = R;
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

int main()
{

    AudioStream stream;
    stream.SetBufSize(audioBufSize, numChannel, sampleRate);
    stream.play();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "TiNRS Wobbler2 Simulator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    SetupTheme();

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (ImGui::Button("Trigger"))
        {
            Trigger = true;
        }
        if (ImGui::Button("BangOn"))
        {
            Trigger = true;
        }
        
        Knob("Freq", &fFreq, 0,1);
        Knob("Mod", &fMod, 0,1);
        Knob("Phase", &fPhase, 0,1);
        Knob("Shape", &fShape, 0,1);
        Knob("NormalAmt", &fNormalAmt, 0,1);
        Knob("PhasedAmt", &fPhasedAmt, 0,1);
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}