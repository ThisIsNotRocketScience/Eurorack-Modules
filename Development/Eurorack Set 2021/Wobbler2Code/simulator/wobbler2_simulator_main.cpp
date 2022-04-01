#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "../Wobbler2Drum.h"
#include "../Wobbler2.cpp"
#include "../Pendulum.cpp"
#include "../../EurorackShared/Math.c"
#include "../../EurorackShared/ImGuiThings.c"
#include <vector>
#include "implot/implot.h"

bool Pattern[4][16] = {false};
float BPM = 150;
bool patternplaying = false;
int tick = 0;

int samplesleftintick = 0;
int sampleRate = 44100;
int numChannel = 2;
int audioBufSize = 2048;
bool UseKnobs;
class Ranger
{
public:
    Ranger()
    {
        Reset();
    }
    void Reset()
    {
        minval = maxval = 0;
    }
    void Add(float v)
    {
        if (v > maxval)
            maxval = v;
        if (v < minval)
            minval = v;
    }
    float minval;
    float maxval;

    void Show()
    {
        //   ImGui::BeginChild("Range", ImVec2(100,140), true, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Min: %.2f / ", (float)minval);
        ImGui::SameLine();
        ImGui::Text("Max: %.2f / ", (float)maxval);
        ImGui::SameLine();
        ImGui::Text("Diff: %.2f / ", (float)(maxval - minval));
        ImGui::SameLine();
        ImGui::Text("Log2: %.2f / ", (float)log2f(maxval - minval));
        if (ImGui::Button("Reset Range"))
        {
            maxval = minval = 0;
        }
        //   ImGui::EndChild();
    }
};

void ShowStepped(uint16_t original, SteppedResult_t *st)
{
    ImGui::Text("%d(%04x) -> %d %.0f%%", (int)original, original, st->index, (st->fractional * 100.0f / 255.0f));
};

class Wobbler2Instance
{
public:
    bool Trigger = false;
    bool UnTrigger = false;

    float fFreq = 0.0f;
    float fMod = 0.0f;
    float fShape = 0.0f;
    float fPhase = 0.0f;
    float fNormalAmt = 0.0f;
    float fPhasedAmt = 0.0f;

    Wobbler2Drum TheDrum;
    Wobbler2_LFO_t Wobbler2;

    float VolumeLevel = 0.8;
    float Panning = 0.5;
    bool Output = false;
    bool change = false;
    void Menu()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        if (ImGui::Button("Trigger"))
        {
            Trigger = true;
        }
        if (ImGui::Button("Stop"))
        {
            UnTrigger = true;
        }
        ImGui::PopStyleVar();
        // if (ImGui::Button("BangOn"))
        //{
        //   Trigger = true;
        // }
        if (UseKnobs)
        {

            if (Knob("Phase", &fPhase, 0, 1))
                change = true;
            ImGui::SameLine();
            if (Knob("Mod", &fMod, 0, 1))
                change = true;

            if (Knob("Freq", &fFreq, 0, 1))
                change = true;
            if (Knob("Shape", &fShape, 0, 1))
                change = true;
            ShowStepped(TheDrum.Shape, &TheDrum.ShapeStepped);
            if (ShowShapeButtons())
                change = true;
            if (Knob("NormalAmt", &fNormalAmt, 0, 1))
                change = true;
            ImGui::SameLine();
            if (Knob("PhasedAmt", &fPhasedAmt, 0, 1))
                change = true;

            if (Knob("Vol", &VolumeLevel, 0, 1))
                change = true;
            ImGui::SameLine();
            if (Knob("Pan", &Panning, 0, 1))
                change = true;
        }
        else
        {
            if (ImGui::SliderFloat("Phase", &fPhase, 0, 1))
                change = true;
            if (ImGui::SliderFloat("Mod", &fMod, 0, 1))
                change = true;

            if (ImGui::SliderFloat("Freq", &fFreq, 0, 1))
                change = true;
            if (ImGui::SliderFloat("Shape", &fShape, 0, 1))
                change = true;
            ShowStepped(TheDrum.Shape, &TheDrum.ShapeStepped);
            if (ShowShapeButtons())
                change = true;

            if (ImGui::SliderFloat("NormalAmt", &fNormalAmt, 0, 1))
                change = true;
            if (ImGui::SliderFloat("PhasedAmt", &fPhasedAmt, 0, 1))
                change = true;

            if (ImGui::SliderFloat("Vol", &VolumeLevel, 0, 1))
                change = true;
            ;
            if (ImGui::SliderFloat("Pan", &Panning, 0, 1))
                change = true;
        }

        ImGui::PopStyleVar();
    }

    bool ShowShapeButtons()
    {
        float fshapestart = fShape;
        if (ImGui::Button("Kick"))
        {
            fShape = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("Tom"))
        {
            fShape = 1 / 6.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Block"))
        {
            fShape = 2 / 6.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Ploink"))
        {
            fShape = 3 / 6.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Snare"))
        {
            fShape = 4 / 6.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Hihat"))
        {
            fShape = 5 / 6.0f;
        }
        ImGui::SameLine();

        if (ImGui::Button("Clap"))
        {
            fShape = 1.0f;
        }

        if (fshapestart != fShape)
            return true;
        return false;
    }
    void SetParam()
    {
        uint16_t uFreq = (int)(fFreq * 0xffff);
        uint16_t uMod = (int)(fMod * 0xffff);
        uint16_t uPhase = (int)(fPhase * 0xffff);
        uint16_t uPhasedAmt = (int)(fPhasedAmt * 0xffff);
        uint16_t uShape = (int)(fShape * 0xffff);
        uint16_t uNormalAmt = (int)(fNormalAmt * 0xffff);
        TheDrum.SetFreq(uFreq);
        TheDrum.SetShape(uShape);
        TheDrum.SetMod(uMod);
        TheDrum.Phase = uPhase;
    }
};

Ranger SampleRange;
Wobbler2Instance Inst[4];
int CurrentInstance = 0;
void WriteFloat(FILE *o, float f)
{
    fwrite(&f, sizeof(float), 1, o);
}

void WriteBool(FILE *o, bool b)
{
    if (b)
    {
        WriteFloat(o, 1.0f);
    }
    else
    {
        WriteFloat(o, 0.0f);
    }
}

void ReadFloat(FILE *o, float *f)
{
    fread(f, sizeof(float), 1, o);
}

bool ReadBool(FILE *o)
{
    float F = 0;
    ReadFloat(o, &F);
    if (F > 0.0f)
        return true;
    return false;
}
void Save()
{
    FILE *o = fopen("save.dat", "wb+");
    if (o == NULL)
        return;
    for (int j = 0; j < 4; j++)
    {
        WriteFloat(o, Inst[j].VolumeLevel);
        WriteFloat(o, Inst[j].Panning);

        WriteFloat(o, Inst[j].fFreq);
        WriteFloat(o, Inst[j].fMod);
        WriteFloat(o, Inst[j].fPhase);
        WriteFloat(o, Inst[j].fShape);
        WriteFloat(o, Inst[j].fPhasedAmt);
        WriteFloat(o, Inst[j].fNormalAmt);
        WriteBool(o, Inst[j].Output);

        for (int i = 0; i < 16; i++)
        {
            WriteBool(o, Pattern[j][i] ? 1.0 : 0.0);
        }
    }
    fclose(o);
}

void Load()
{
    FILE *o = fopen("save.dat", "rb+");
    if (o == NULL)
        return;
    for (int j = 0; j < 4; j++)
    {
        ReadFloat(o, &Inst[j].VolumeLevel);
        ReadFloat(o, &Inst[j].Panning);

        ReadFloat(o, &Inst[j].fFreq);
        ReadFloat(o, &Inst[j].fMod);
        ReadFloat(o, &Inst[j].fPhase);
        ReadFloat(o, &Inst[j].fShape);
        ReadFloat(o, &Inst[j].fPhasedAmt);
        ReadFloat(o, &Inst[j].fNormalAmt);

        Inst[j].Output = ReadBool(o);
        for (int i = 0; i < 16; i++)
        {
            Pattern[j][i] = ReadBool(o);
        }
    }
    fclose(o);
}

class DecayEnvTester
{
public:
    std::vector<float> results_ClapRattle;
    std::vector<float> results_BdDecay;
    std::vector<float> results_PDecay;
    std::vector<float> results_SnareNoiseAmp;

    RattleEnv ClapRattle;
    DecayEnv SnareNoiseAmp;
    DecayEnv BdDecay;
    DecayEnv PDecay;

    std::vector<float> indices;
    Ranger EnvelopeRange;
    bool recalcenvelopes = true;
    void ShowMenu()
    {
        ImGui::Begin("Envelopes", NULL, 0);
        if (ImGui::Button("retrigger") || recalcenvelopes)
        {
            recalcenvelopes = false;
            SnareNoiseAmp.Trigger();
            BdDecay.CopyFrom(&Inst[0].TheDrum.BdDecay);
            PDecay.CopyFrom(&Inst[0].TheDrum.PDecay);
            ClapRattle.CopyFrom(&Inst[0].TheDrum.ClapRattle);
            SnareNoiseAmp.CopyFrom(&Inst[0].TheDrum.SnareNoiseAmp);
            PDecay.Trigger();
            BdDecay.Trigger();
            SnareNoiseAmp.Trigger();

            ClapRattle.Trigger();

            results_ClapRattle.clear();
            results_BdDecay.clear();
            results_PDecay.clear();
            results_SnareNoiseAmp.clear();
            indices.clear();
            for (int i = 0; i < 3000; i++)
            {
                indices.push_back(i);
                EnvelopeRange.Add(ClapRattle.EnvCurrent);
                results_SnareNoiseAmp.push_back(SnareNoiseAmp.Get());
                results_ClapRattle.push_back(ClapRattle.Get());
                results_PDecay.push_back(PDecay.Get());
                results_BdDecay.push_back(BdDecay.Get());
            }
        }

        EnvelopeRange.Show();

        // ImPlot::PushStyleVar(ImPlotst)
        if (ImGui::CollapsingHeader("PitchEnv"))
        {
            ImPlot::SetNextAxesToFit();
            if (ImPlot::BeginPlot("Pitch Mod Envelope"))
            {
                ImPlot::PlotLine("PDecay", &indices[0], &results_PDecay[0], indices.size());
                ImPlot::EndPlot();
            }
        }
        if (ImGui::CollapsingHeader("Bd & Tom Decay Env"))
        {

            ImPlot::SetNextAxesToFit();
            if (ImPlot::BeginPlot("BdDecay"))
            {
                ImPlot::PlotLine("BdDecay", &indices[0], &results_BdDecay[0], indices.size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }
};
DecayEnvTester DecayTest;

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
        for (int j = 0; j < 4; j++)
        {
            if (Inst[j].Trigger)
            {
                Inst[j].TheDrum.Trigger(true);
                Inst[j].Trigger = false;
            }

            if (Inst[j].UnTrigger)
            {
                Inst[j].TheDrum.Trigger(false);
                Inst[j].UnTrigger = false; 
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
                samplesleftintick--;
                if (samplesleftintick <= 0)
                {
                    tick = (tick + 1) % 16;
                    for (int j = 0; j < 4; j++)
                    {
                        if (Pattern[j][tick])
                        {
                            Inst[j].TheDrum.Trigger();
                        }
                    }
                    samplesleftintick = ((44100 * 60) / (BPM * 4));
                }
            }
            for (int j = 0; j < 4; j++)
            {
                int A = 0, B = 0;
                Inst[j].TheDrum.Get(A, B);
                float L = cosf(Inst[j].Panning * 3.1415 / 2);
                float R = cosf((1 - Inst[j].Panning) * 3.1415 / 2);
                if (Inst[j].Output)
                {
                    L *= A;
                    R *= A;
                }
                else
                {
                    L *= B;
                    R *= B;
                }
                m_samples[i] += L * Inst[j].VolumeLevel * (0.1 * 32767.0f / 2048.0f);
                m_samples[i + 1] += R * Inst[j].VolumeLevel * (0.1 * 32767.0f / 2048.0f);
                SampleRange.Add(m_samples[i]);
                SampleRange.Add(m_samples[i + 1]);
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

    Load();
    sf::RenderWindow window(sf::VideoMode(1680, 1020), "TiNRS Wobbler2 Simulator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    ImPlot::CreateContext();

    ImPlotStyle &ImPlot_Style = ImPlot::GetStyle();
    ImPlot_Style.UseLocalTime = true;
    ImPlot_Style.AntiAliasedLines = true;

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
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Knobs", 0, &UseKnobs);

            if (ImGui::MenuItem("Save"))
            {
                Save();
            }

            if (ImGui::MenuItem("Load"))
            {
                Load();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        ImGui::PopFont();

        ImGui::PushFont(pFontBold);

        ImGui::Begin("Wobbler2", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        int lastinstance = CurrentInstance;
        if (ImGui::RadioButton("1##inst1", CurrentInstance == 0))
            CurrentInstance = 0;
        ImGui::SameLine();
        if (ImGui::RadioButton("2##inst2", CurrentInstance == 1))
            CurrentInstance = 1;
        ImGui::SameLine();
        if (ImGui::RadioButton("3##inst3", CurrentInstance == 2))
            CurrentInstance = 2;
        ImGui::SameLine();
        if (ImGui::RadioButton("4##inst4", CurrentInstance == 3))
            CurrentInstance = 3;

        if (lastinstance != CurrentInstance)
            DecayTest.recalcenvelopes = true;
        ImGui::BeginGroup();
        Inst[CurrentInstance].Menu();

        ImGui::EndGroup();
        ImGui::End();
        if (Inst[CurrentInstance].change)
        {
            printf("recalc env display!\n");
            Inst[CurrentInstance].change = false;
            DecayTest.recalcenvelopes = true;
        }
        DecayTest.ShowMenu();
        ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        SampleRange.Show();
        ImGui::End();
        ImGui::Begin("Sequencer", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        if(ImGui::Checkbox("Play!", &patternplaying))
        {
            tick = -1;
            samplesleftintick = -1;
        }
        ImGui::SameLine();

        ImGui::SliderFloat("BPM", &BPM, 20, 240);

        // ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, ImVec2(4,4));
        for (int j = 0; j < 4; j++)
        {
            char txt[40];
            sprintf(txt, "##r%d", j + 1, j + 1);

            if (ImGui::RadioButton(txt, CurrentInstance == j))
            {
                CurrentInstance = j;
            }
            ImGui::SameLine();

            for (int i = 0; i < 16; i++)
            {
                if (tick == i)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 1, 0, 1));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.6, 0.6, 0.6, 1.0));
                }

                sprintf(txt, "##%d_%d", i, j);
                if (ImGui::Checkbox(txt, &Pattern[j][i]))
                {
                    CurrentInstance = j;
                }
                ImGui::PopStyleColor();
                if (i < 15)
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
    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();

    return 0;
}