void ATLAS_razor_1_jetMult()
{
//=========Macro generated from canvas: Canvas/
//=========  (Tue Aug 19 10:01:34 2014) by ROOT version5.99/06
   TCanvas *Canvas = new TCanvas("Canvas", "",1,1,700,476);
   Canvas->SetHighLightColor(2);
   Canvas->Range(-1.25,-1620.413,11.25,14583.71);
   Canvas->SetFillColor(0);
   Canvas->SetBorderMode(0);
   Canvas->SetBorderSize(2);
   Canvas->SetFrameBorderMode(0);
   Canvas->SetFrameBorderMode(0);
   
   TH1F *jetMult3 = new TH1F("jetMult3","Jet Multiplicity",10,0,10);
   jetMult3->SetBinContent(1,12346);
   jetMult3->SetBinContent(2,7371);
   jetMult3->SetBinContent(3,2606);
   jetMult3->SetBinContent(4,805);
   jetMult3->SetBinContent(5,281);
   jetMult3->SetBinContent(6,83);
   jetMult3->SetBinContent(7,19);
   jetMult3->SetBinContent(8,9);
   jetMult3->SetBinContent(9,1);
   jetMult3->SetEntries(23521);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *AText = ptstats->AddText("jetMult");
   AText->SetTextSize(0.0368);
   AText = ptstats->AddText("Entries = 23521  ");
   AText = ptstats->AddText("Mean  = 0.7109");
   AText = ptstats->AddText("RMS   = 0.9429");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   jetMult3->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(jetMult3);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   jetMult3->SetLineColor(ci);
   jetMult3->GetXaxis()->SetTitle("Jet Multiplicity");
   jetMult3->GetXaxis()->SetLabelFont(42);
   jetMult3->GetXaxis()->SetLabelSize(0.035);
   jetMult3->GetXaxis()->SetTitleSize(0.035);
   jetMult3->GetXaxis()->SetTitleFont(42);
   jetMult3->GetYaxis()->SetTitle("Occurrence");
   jetMult3->GetYaxis()->SetLabelFont(42);
   jetMult3->GetYaxis()->SetLabelSize(0.035);
   jetMult3->GetYaxis()->SetTitleSize(0.035);
   jetMult3->GetYaxis()->SetTitleFont(42);
   jetMult3->GetZaxis()->SetLabelFont(42);
   jetMult3->GetZaxis()->SetLabelSize(0.035);
   jetMult3->GetZaxis()->SetTitleSize(0.035);
   jetMult3->GetZaxis()->SetTitleFont(42);
   jetMult3->Draw("");
   
   TPaveText *pt = new TPaveText(0.3700287,0.9339831,0.6299713,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   AText = pt->AddText("Jet Multiplicity");
   pt->Draw();
   Canvas->Modified();
   Canvas->cd();
   Canvas->SetSelected(Canvas);
}
