<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="16" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="erthenvar">
<packages>
<package name="ERTHENVAR-JACK">
<pad name="1@1" x="-4.2" y="0" drill="2.5" rot="R90"/>
<pad name="3" x="0" y="4.9" drill="2.5" rot="R90"/>
<pad name="2" x="0" y="-3.3" drill="2.5" rot="R90"/>
<pad name="1@2" x="4.2" y="0" drill="1.5" rot="R90"/>
<text x="-3.19" y="-8.65" size="1.27" layer="21">&gt;NAME</text>
<circle x="0" y="0" radius="1" width="0.127" layer="21"/>
<circle x="0" y="0" radius="2" width="0.5" layer="21"/>
<circle x="0" y="0" radius="3" width="0.127" layer="21"/>
<circle x="0" y="0" radius="4" width="2" layer="21"/>
<circle x="0" y="0" radius="6.403121875" width="0.3" layer="21"/>
<circle x="0" y="0" radius="1" width="0.127" layer="22"/>
<circle x="0" y="0" radius="2" width="0.5" layer="22"/>
<circle x="0" y="0" radius="3" width="0.127" layer="22"/>
<circle x="0" y="0" radius="4" width="2" layer="22"/>
<circle x="0" y="0" radius="6.403121875" width="0.3" layer="22"/>
</package>
</packages>
<symbols>
<symbol name="AUDIO-JACK">
<wire x1="-5.08" y1="2.54" x2="5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-5.08" x2="-1.27" y2="-3.81" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-3.81" x2="0" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="5.08" y2="-5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="0" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="-3.81" x2="1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="3.81" y2="-2.54" width="0.254" layer="94"/>
<wire x1="3.81" y1="-2.54" x2="2.54" y2="-3.81" width="0.254" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="2.54" y2="0" width="0.254" layer="94"/>
<rectangle x1="-7.62" y1="-5.08" x2="-5.08" y2="2.54" layer="94"/>
<pin name="RING" x="7.62" y="2.54" length="middle" rot="R180"/>
<pin name="NORM" x="7.62" y="0" length="middle" rot="R180"/>
<pin name="TIP" x="7.62" y="-5.08" length="middle" rot="R180"/>
<text x="-7.493" y="4.445" size="1.778" layer="95">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="AUDIO-JACK">
<description>Switching Mono 3.5mm Audio Jack from Erthenvar.</description>
<gates>
<gate name="G$1" symbol="AUDIO-JACK" x="0" y="0"/>
</gates>
<devices>
<device name="ERTHENVAR" package="ERTHENVAR-JACK">
<connects>
<connect gate="G$1" pin="NORM" pad="2"/>
<connect gate="G$1" pin="RING" pad="1@1 1@2"/>
<connect gate="G$1" pin="TIP" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="EurorackModularParts">
<packages>
<package name="EURORACK2HP">
<wire x1="1" y1="128.5" x2="9.16" y2="128.5" width="0" layer="20"/>
<wire x1="9.16" y1="128.5" x2="10.16" y2="127.5" width="0" layer="20" curve="-90"/>
<wire x1="10.16" y1="127.5" x2="10.16" y2="1" width="0" layer="20"/>
<wire x1="10.16" y1="1" x2="9.16" y2="0" width="0" layer="20" curve="-90"/>
<wire x1="9.16" y1="0" x2="1" y2="0" width="0" layer="20"/>
<wire x1="1" y1="0" x2="0" y2="1" width="0" layer="20" curve="-90"/>
<wire x1="0" y1="1" x2="0" y2="127.5" width="0" layer="20"/>
<wire x1="0" y1="127.5" x2="1" y2="128.5" width="0" layer="20" curve="-90"/>
<hole x="7.5" y="3" drill="3.2"/>
<hole x="7.5" y="125.5" drill="3.2"/>
</package>
<package name="EURORACK4HP">
<wire x1="1" y1="128.5" x2="19.32" y2="128.5" width="0" layer="20"/>
<wire x1="19.32" y1="128.5" x2="20.32" y2="127.5" width="0" layer="20" curve="-90"/>
<wire x1="20.32" y1="127.5" x2="20.32" y2="1" width="0" layer="20"/>
<wire x1="20.32" y1="1" x2="19.32" y2="0" width="0" layer="20" curve="-90"/>
<wire x1="19.32" y1="0" x2="1" y2="0" width="0" layer="20"/>
<wire x1="1" y1="0" x2="0" y2="1" width="0" layer="20" curve="-90"/>
<wire x1="0" y1="1" x2="0" y2="127.5" width="0" layer="20"/>
<wire x1="0" y1="127.5" x2="1" y2="128.5" width="0" layer="20" curve="-90"/>
<hole x="7.5" y="3" drill="3.2"/>
<hole x="7.5" y="125.5" drill="3.2"/>
</package>
<package name="EURORACK6HP">
<wire x1="1" y1="128.5" x2="29.48" y2="128.5" width="0" layer="20"/>
<wire x1="29.48" y1="128.5" x2="30.48" y2="127.5" width="0" layer="20" curve="-90"/>
<wire x1="30.48" y1="127.5" x2="30.48" y2="1" width="0" layer="20"/>
<wire x1="30.48" y1="1" x2="29.48" y2="0" width="0" layer="20" curve="-90"/>
<wire x1="29.48" y1="0" x2="1" y2="0" width="0" layer="20"/>
<wire x1="1" y1="0" x2="0" y2="1" width="0" layer="20" curve="-90"/>
<wire x1="0" y1="1" x2="0" y2="127.5" width="0" layer="20"/>
<wire x1="0" y1="127.5" x2="1" y2="128.5" width="0" layer="20" curve="-90"/>
<hole x="7.5" y="3" drill="3.2"/>
<hole x="7.5" y="125.5" drill="3.2"/>
</package>
<package name="EURORACK12HP">
<wire x1="1" y1="128.5" x2="59.96" y2="128.5" width="0" layer="20"/>
<wire x1="59.96" y1="0" x2="1" y2="0" width="0" layer="20"/>
<wire x1="1" y1="0" x2="0" y2="1" width="0" layer="20" curve="-90"/>
<wire x1="0" y1="1" x2="0" y2="127.5" width="0" layer="20"/>
<wire x1="0" y1="127.5" x2="1" y2="128.5" width="0" layer="20" curve="-90"/>
<hole x="7.5" y="3" drill="3.2"/>
<hole x="7.5" y="125.5" drill="3.2"/>
<wire x1="59.96" y1="128.5" x2="60.96" y2="127.5" width="0" layer="20" curve="-90"/>
<wire x1="60.96" y1="127.5" x2="60.96" y2="1" width="0" layer="20"/>
<wire x1="60.96" y1="1" x2="59.96" y2="0" width="0" layer="20" curve="-90"/>
<hole x="53.22" y="3" drill="3.2"/>
<hole x="53.22" y="125.5" drill="3.2"/>
</package>
<package name="EURORACK8HPTEMPLATE">
<wire x1="0" y1="105.5" x2="0" y2="1" width="0.127" layer="20"/>
<wire x1="0" y1="1" x2="1" y2="0" width="0.127" layer="20" curve="90"/>
<wire x1="1" y1="0" x2="38.3" y2="0" width="0.127" layer="20"/>
<wire x1="38.3" y1="0" x2="39.3" y2="1" width="0.127" layer="20" curve="90"/>
<wire x1="39.3" y1="1" x2="39.3" y2="105.5" width="0.127" layer="20"/>
<wire x1="39.3" y1="105.5" x2="38.3" y2="106.5" width="0.127" layer="20" curve="90"/>
<wire x1="38.3" y1="106.5" x2="1" y2="106.5" width="0.127" layer="20"/>
<wire x1="1" y1="106.5" x2="0" y2="105.5" width="0.127" layer="20" curve="90"/>
<wire x1="-0.5" y1="-11" x2="39.8" y2="-11" width="0" layer="51"/>
<wire x1="39.8" y1="-11" x2="39.8" y2="117.5" width="0" layer="51"/>
<wire x1="39.8" y1="117.5" x2="-0.5" y2="117.5" width="0" layer="51"/>
<wire x1="-0.5" y1="117.5" x2="-0.5" y2="-11" width="0" layer="51"/>
<text x="0" y="115" size="1.27" layer="51">8HP front template</text>
<wire x1="9.575" y1="130" x2="9.575" y2="-20" width="0" layer="51"/>
<wire x1="29.725" y1="130" x2="29.725" y2="-20" width="0" layer="51"/>
<wire x1="12.933" y1="130" x2="12.933" y2="-20" width="0" layer="51"/>
<wire x1="19.65" y1="130" x2="19.65" y2="-20" width="0" layer="51"/>
<wire x1="26.37" y1="130" x2="26.37" y2="-20" width="0" layer="51"/>
<text x="41" y="85" size="1.27" layer="51">2 columns:
12,933
26,37

3 columns:
9,575  
19,65
29,725

PCB width: 
39,3</text>
</package>
</packages>
<symbols>
<symbol name="EURORACK_PANEL">
<text x="0" y="0" size="1.27" layer="94">EURORACK PANEL</text>
<wire x1="-2.54" y1="-12.7" x2="-2.54" y2="12.7" width="0.254" layer="94"/>
<wire x1="-2.54" y1="12.7" x2="17.78" y2="12.7" width="0.254" layer="94"/>
<wire x1="17.78" y1="12.7" x2="17.78" y2="-12.7" width="0.254" layer="94"/>
<wire x1="17.78" y1="-12.7" x2="-2.54" y2="-12.7" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="EURORACK_PANEL">
<gates>
<gate name="G$1" symbol="EURORACK_PANEL" x="0" y="0"/>
</gates>
<devices>
<device name="2HP" package="EURORACK2HP">
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="4HP" package="EURORACK4HP">
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="6HP" package="EURORACK6HP">
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="12HP" package="EURORACK12HP">
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="8HPPCB" package="EURORACK8HPTEMPLATE">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
<class number="1" name="AUDIO" width="1.016" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$2" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$3" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$4" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$5" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$6" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$7" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$8" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$9" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$10" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$11" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$12" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$13" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$14" library="erthenvar" deviceset="AUDIO-JACK" device="ERTHENVAR"/>
<part name="U$15" library="EurorackModularParts" deviceset="EURORACK_PANEL" device="8HPPCB"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="22.86" y="91.44"/>
<instance part="U$2" gate="G$1" x="22.86" y="78.74"/>
<instance part="U$3" gate="G$1" x="22.86" y="63.5"/>
<instance part="U$4" gate="G$1" x="22.86" y="50.8"/>
<instance part="U$5" gate="G$1" x="63.5" y="91.44"/>
<instance part="U$6" gate="G$1" x="63.5" y="78.74"/>
<instance part="U$7" gate="G$1" x="63.5" y="63.5"/>
<instance part="U$8" gate="G$1" x="22.86" y="25.4"/>
<instance part="U$9" gate="G$1" x="66.04" y="25.4"/>
<instance part="U$10" gate="G$1" x="22.86" y="7.62"/>
<instance part="U$11" gate="G$1" x="66.04" y="7.62"/>
<instance part="U$12" gate="G$1" x="22.86" y="-10.16"/>
<instance part="U$13" gate="G$1" x="66.04" y="-10.16"/>
<instance part="U$14" gate="G$1" x="22.86" y="-27.94"/>
<instance part="U$15" gate="G$1" x="119.38" y="50.8"/>
</instances>
<busses>
</busses>
<nets>
<net name="RINGS" class="1">
<segment>
<pinref part="U$2" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="81.28" x2="45.72" y2="81.28" width="0.1524" layer="91"/>
<label x="38.1" y="81.28" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="53.34" x2="45.72" y2="53.34" width="0.1524" layer="91"/>
<label x="38.1" y="53.34" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="RING"/>
<wire x1="71.12" y1="93.98" x2="86.36" y2="93.98" width="0.1524" layer="91"/>
<label x="78.74" y="93.98" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="RING"/>
<wire x1="71.12" y1="66.04" x2="86.36" y2="66.04" width="0.1524" layer="91"/>
<label x="78.74" y="66.04" size="1.778" layer="95"/>
</segment>
</net>
<net name="TIPS" class="1">
<segment>
<pinref part="U$2" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="73.66" x2="45.72" y2="73.66" width="0.1524" layer="91"/>
<label x="38.1" y="73.66" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="45.72" x2="45.72" y2="45.72" width="0.1524" layer="91"/>
<label x="38.1" y="45.72" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="TIP"/>
<wire x1="71.12" y1="86.36" x2="86.36" y2="86.36" width="0.1524" layer="91"/>
<label x="78.74" y="86.36" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="TIP"/>
<wire x1="71.12" y1="58.42" x2="86.36" y2="58.42" width="0.1524" layer="91"/>
<label x="78.74" y="58.42" size="1.778" layer="95"/>
</segment>
</net>
<net name="RINGS2" class="1">
<segment>
<pinref part="U$8" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="27.94" x2="45.72" y2="27.94" width="0.1524" layer="91"/>
<label x="38.1" y="27.94" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="RING"/>
<wire x1="73.66" y1="27.94" x2="88.9" y2="27.94" width="0.1524" layer="91"/>
<label x="81.28" y="27.94" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="10.16" x2="45.72" y2="10.16" width="0.1524" layer="91"/>
<label x="38.1" y="10.16" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="RING"/>
<wire x1="73.66" y1="10.16" x2="88.9" y2="10.16" width="0.1524" layer="91"/>
<label x="81.28" y="10.16" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="-7.62" x2="45.72" y2="-7.62" width="0.1524" layer="91"/>
<label x="38.1" y="-7.62" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="RING"/>
<wire x1="73.66" y1="-7.62" x2="88.9" y2="-7.62" width="0.1524" layer="91"/>
<label x="81.28" y="-7.62" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="-25.4" x2="45.72" y2="-25.4" width="0.1524" layer="91"/>
<label x="38.1" y="-25.4" size="1.778" layer="95"/>
</segment>
</net>
<net name="TIPS2" class="1">
<segment>
<pinref part="U$8" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="20.32" x2="45.72" y2="20.32" width="0.1524" layer="91"/>
<label x="38.1" y="20.32" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="TIP"/>
<wire x1="73.66" y1="20.32" x2="88.9" y2="20.32" width="0.1524" layer="91"/>
<label x="81.28" y="20.32" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="2.54" x2="45.72" y2="2.54" width="0.1524" layer="91"/>
<label x="38.1" y="2.54" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="TIP"/>
<wire x1="73.66" y1="2.54" x2="88.9" y2="2.54" width="0.1524" layer="91"/>
<label x="81.28" y="2.54" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="-15.24" x2="45.72" y2="-15.24" width="0.1524" layer="91"/>
<label x="38.1" y="-15.24" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="TIP"/>
<wire x1="73.66" y1="-15.24" x2="88.9" y2="-15.24" width="0.1524" layer="91"/>
<label x="81.28" y="-15.24" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="-33.02" x2="45.72" y2="-33.02" width="0.1524" layer="91"/>
<label x="38.1" y="-33.02" size="1.778" layer="95"/>
</segment>
</net>
<net name="RINGS3" class="1">
<segment>
<pinref part="U$6" gate="G$1" pin="RING"/>
<wire x1="71.12" y1="81.28" x2="86.36" y2="81.28" width="0.1524" layer="91"/>
<label x="78.74" y="81.28" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="66.04" x2="45.72" y2="66.04" width="0.1524" layer="91"/>
<label x="38.1" y="66.04" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="RING"/>
<wire x1="30.48" y1="93.98" x2="45.72" y2="93.98" width="0.1524" layer="91"/>
<label x="38.1" y="93.98" size="1.778" layer="95"/>
</segment>
</net>
<net name="TIPS3" class="1">
<segment>
<pinref part="U$6" gate="G$1" pin="TIP"/>
<wire x1="71.12" y1="73.66" x2="86.36" y2="73.66" width="0.1524" layer="91"/>
<label x="78.74" y="73.66" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="58.42" x2="45.72" y2="58.42" width="0.1524" layer="91"/>
<label x="38.1" y="58.42" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="TIP"/>
<wire x1="30.48" y1="86.36" x2="45.72" y2="86.36" width="0.1524" layer="91"/>
<label x="38.1" y="86.36" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="6.3" minversion="6.2.2" severity="warning">
Since Version 6.2.2 text objects can contain more than one line,
which will not be processed correctly with this version.
</note>
</compatibility>
</eagle>
