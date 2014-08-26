void ATLAS_razor_1_otherJetPt()
{
//=========Macro generated from canvas: Canvas/
//=========  (Tue Aug 19 10:01:34 2014) by ROOT version5.99/06
   TCanvas *Canvas = new TCanvas("Canvas", "",1,1,700,476);
   Canvas->SetHighLightColor(2);
   Canvas->Range(-125,-139.5188,1125,1255.669);
   Canvas->SetFillColor(0);
   Canvas->SetBorderMode(0);
   Canvas->SetBorderSize(2);
   Canvas->SetFrameBorderMode(0);
   Canvas->SetFrameBorderMode(0);
   
   TH1F *otherJetPt6 = new TH1F("otherJetPt6","Other Jet PT",100,0,1000);
   otherJetPt6->SetBinContent(3,1063);
   otherJetPt6->SetBinContent(4,332);
   otherJetPt6->SetBinContent(5,157);
   otherJetPt6->SetBinContent(6,75);
   otherJetPt6->SetBinContent(7,38);
   otherJetPt6->SetBinContent(8,34);
   otherJetPt6->SetBinContent(9,15);
   otherJetPt6->SetBinContent(10,12);
   otherJetPt6->SetBinContent(11,7);
   otherJetPt6->SetBinContent(12,1);
   otherJetPt6->SetBinContent(13,2);
   otherJetPt6->SetBinContent(15,3);
   otherJetPt6->SetBinContent(16,3);
   otherJetPt6->SetBinContent(19,1);
   otherJetPt6->SetEntries(1743);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *AText = ptstats->AddText("otherJetPt");
   AText->SetTextSize(0.0368);
   AText = ptstats->AddText("Entries = 1743   ");
   AText = ptstats->AddText("Mean  =  32.93");
   AText = ptstats->AddText("RMS   =  16.94");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   otherJetPt6->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(otherJetPt6);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   otherJetPt6->SetLineColor(ci);
   otherJetPt6->GetXaxis()->SetTitle("Other Jet PT (GeV)");
   otherJetPt6->GetXaxis()->SetLabelFont(42);
   otherJetPt6->GetXaxis()->SetLabelSize(0.035);
   otherJetPt6->GetXaxis()->SetTitleSize(0.035);
   otherJetPt6->GetXaxis()->SetTitleFont(42);
   otherJetPt6->GetYaxis()->SetTitle("Occurrence");
   otherJetPt6->GetYaxis()->SetLabelFont(42);
   otherJetPt6->GetYaxis()->SetLabelSize(0.035);
   otherJetPt6->GetYaxis()->SetTitleSize(0.035);
   otherJetPt6->GetYaxis()->SetTitleFont(42);
   otherJetPt6->GetZaxis()->SetLabelFont(42);
   otherJetPt6->GetZaxis()->SetLabelSize(0.035);
   otherJetPt6->GetZaxis()->SetTitleSize(0.035);
   otherJetPt6->GetZaxis()->SetTitleFont(42);
   otherJetPt6->Draw("");
   
   TPaveText *pt = new TPaveText(0.3872701,0.94,0.6127299,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   AText = pt->AddText("Other Jet PT");
   pt->Draw();
   Canvas->Modified();
   Canvas->cd();
   Canvas->SetSelected(Canvas);
}
