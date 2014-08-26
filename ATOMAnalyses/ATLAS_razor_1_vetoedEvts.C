void ATLAS_razor_1_vetoedEvts()
{
//=========Macro generated from canvas: Canvas/
//=========  (Tue Aug 19 10:01:34 2014) by ROOT version5.99/06
   TCanvas *Canvas = new TCanvas("Canvas", "",1,1,700,476);
   Canvas->SetHighLightColor(2);
   Canvas->Range(-0.25,815.3395,2.25,22745.44);
   Canvas->SetFillColor(0);
   Canvas->SetBorderMode(0);
   Canvas->SetBorderSize(2);
   Canvas->SetFrameBorderMode(0);
   Canvas->SetFrameBorderMode(0);
   
   TH1F *vetoedEvts2 = new TH1F("vetoedEvts2","Vetoed Events",2,0,2);
   vetoedEvts2->SetBinContent(1,3804);
   vetoedEvts2->SetBinContent(2,19717);
   vetoedEvts2->SetEntries(23521);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *AText = ptstats->AddText("vetoedEvts");
   AText->SetTextSize(0.0368);
   AText = ptstats->AddText("Entries = 23521  ");
   AText = ptstats->AddText("Mean  = 0.8383");
   AText = ptstats->AddText("RMS   = 0.3682");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   vetoedEvts2->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(vetoedEvts2);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   vetoedEvts2->SetLineColor(ci);
   vetoedEvts2->GetXaxis()->SetTitle("Vetoed Events");
   vetoedEvts2->GetXaxis()->SetLabelFont(42);
   vetoedEvts2->GetXaxis()->SetLabelSize(0.035);
   vetoedEvts2->GetXaxis()->SetTitleSize(0.035);
   vetoedEvts2->GetXaxis()->SetTitleFont(42);
   vetoedEvts2->GetYaxis()->SetTitle("Occurrence");
   vetoedEvts2->GetYaxis()->SetLabelFont(42);
   vetoedEvts2->GetYaxis()->SetLabelSize(0.035);
   vetoedEvts2->GetYaxis()->SetTitleSize(0.035);
   vetoedEvts2->GetYaxis()->SetTitleFont(42);
   vetoedEvts2->GetZaxis()->SetLabelFont(42);
   vetoedEvts2->GetZaxis()->SetLabelSize(0.035);
   vetoedEvts2->GetZaxis()->SetTitleSize(0.035);
   vetoedEvts2->GetZaxis()->SetTitleFont(42);
   vetoedEvts2->Draw("");
   
   TPaveText *pt = new TPaveText(0.3714655,0.94,0.6285345,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   AText = pt->AddText("Vetoed Events");
   pt->Draw();
   Canvas->Modified();
   Canvas->cd();
   Canvas->SetSelected(Canvas);
}
