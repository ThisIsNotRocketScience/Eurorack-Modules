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



bool Pattern[4][16] = {false};
float BPM = 130;
bool patternplaying = false;
int tick =0;;
int samplesleftintick = 0;
int sampleRate = 44100;
int numChannel = 2;
int audioBufSize = 2048;

class Wobbler2Instance
{
    public:
    
    bool Trigger = false;

    float fFreq = 0.0f;
    float fMod = 0.0f;
    float fShape = 0.0f;
    float fPhase = 0.0f;
    float fNormalAmt = 0.0f;
    float fPhasedAmt = 0.0f;

    Wobbler2Drum TheDrum;
    
    float VolumeLevel = 0.8;
    float Panning = 0.5;
    bool Output = false;

    void Menu()
    {
        
        if (ImGui::Button("Trigger"))
        {
            Trigger = true;
        }
        if (ImGui::Button("BangOn"))
        {
            Trigger = true;
        }
        Knob("Phase", &fPhase, 0, 1); ImGui::SameLine();
        Knob("Mod", &fMod, 0, 1);
       
        Knob("Freq", &fFreq, 0, 1);
        Knob("Shape", &fShape, 0, 1);
        Knob("NormalAmt", &fNormalAmt, 0, 1); ImGui::SameLine();
        Knob("PhasedAmt", &fPhasedAmt, 0, 1);
    }
    void SetParam()
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

    }
};
Wobbler2Instance Inst[4];
int CurrentInstance = 0;

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

        Inst[0].SetParam();
        Inst[1].SetParam();
        Inst[2].SetParam();
        Inst[3].SetParam();
        for (int j =0 ;j<4;j++)
        {
        if (Inst[j].Trigger)
        {
            Inst[j].TheDrum.Trigger();
            Inst[j].Trigger = false;
        }

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
            if (patternplaying)
            {
                samplesleftintick --;
                if (samplesleftintick <=0)
                {
                    tick= (tick + 1)%16; 
                    for(int j=0;j<4;j++)
                    {
                    if (Pattern[j][tick])
                    {
                        Inst[j].TheDrum.Trigger();
                    }
                    }
                    samplesleftintick = ((44100*60)/(BPM*4));
                }
            }
            for(int j = 0;j<4;j++)
            {
            int A = 0, B = 0;
            Inst[j].TheDrum.Get(A, B);
            if (Inst[j].Output)
            {

            }
            m_samples[i] += L;
            m_samples[i + 1] += R;
            }
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

    SetupTheme(2.4);

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

        ImGui::PushFont(pFont);
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Parameters"))
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        ImGui::PopFont();

        ImGui::PushFont(pFontBold);

        ImGui::Begin("Wobbler2", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        
        if (ImGui::RadioButton("1##inst1",CurrentInstance == 0 )) CurrentInstance = 0;
        ImGui::SameLine();
        if (ImGui::RadioButton("2##inst2",CurrentInstance == 1 )) CurrentInstance = 1;
        ImGui::SameLine();
        if (ImGui::RadioButton("3##inst3",CurrentInstance ==  2)) CurrentInstance = 2;
        ImGui::SameLine();
        if (ImGui::RadioButton("4##inst4",CurrentInstance == 3 )) CurrentInstance = 3;
        Inst[CurrentInstance].Menu();
        ImGui::End();

        ImGui::Begin("Sequencer",NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Checkbox("Play!", &patternplaying);
        //ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, ImVec2(4,4));
        for(int j =0 ;j<4;j++)
        {

        
        for(int i =0 ;i<16;i++)
        {
            if (tick == i)
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1,1,0,1));
            }
            else{
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.6,0.6,0.6,1.0));
                
            }
            char txt[40];
            sprintf(txt, "##%d_%d", i,j);
            ImGui::Checkbox(txt, &Pattern[j][i]);
            ImGui::PopStyleColor();
            if (i<15)
            {
                ImGui::SameLine();
            }
        }
        }
        ImGui::End();
        ImGui::PopFont();
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}