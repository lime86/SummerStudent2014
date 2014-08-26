void ATLAS_razor_1_subleadingJetPt()
{
//=========Macro generated from canvas: Canvas/
//=========  (Tue Aug 19 10:01:34 2014) by ROOT version5.99/06
   TCanvas *Canvas = new TCanvas("Canvas", "",1,1,700,476);
   Canvas->SetHighLightColor(2);
   Canvas->Range(-250,-231.3938,2250,2082.544);
   Canvas->SetFillColor(0);
   Canvas->SetBorderMode(0);
   Canvas->SetBorderSize(2);
   Canvas->SetFrameBorderMode(0);
   Canvas->SetFrameBorderMode(0);
   
   TH1F *subleadingJetPt5 = new TH1F("subleadingJetPt5","Subleading Jet PT",200,0,2000);
   subleadingJetPt5->SetBinContent(3,1763);
   subleadingJetPt5->SetBinContent(4,811);
   subleadingJetPt5->SetBinContent(5,411);
   subleadingJetPt5->SetBinContent(6,235);
   subleadingJetPt5->SetBinContent(7,133);
   subleadingJetPt5->SetBinContent(8,103);
   subleadingJetPt5->SetBinContent(9,61);
   subleadingJetPt5->SetBinContent(10,66);
   subleadingJetPt5->SetBinContent(11,48);
   subleadingJetPt5->SetBinContent(12,32);
   subleadingJetPt5->SetBinContent(13,18);
   subleadingJetPt5->SetBinContent(14,19);
   subleadingJetPt5->SetBinContent(15,18);
   subleadingJetPt5->SetBinContent(16,18);
   subleadingJetPt5->SetBinContent(17,17);
   subleadingJetPt5->SetBinContent(18,7);
   subleadingJetPt5->SetBinContent(19,6);
   subleadingJetPt5->SetBinContent(20,8);
   subleadingJetPt5->SetBinContent(21,5);
   subleadingJetPt5->SetBinContent(22,4);
   subleadingJetPt5->SetBinContent(23,5);
   subleadingJetPt5->SetBinContent(24,4);
   subleadingJetPt5->SetBinContent(26,1);
   subleadingJetPt5->SetBinContent(27,2);
   subleadingJetPt5->SetBinContent(28,1);
   subleadingJetPt5->SetBinContent(29,2);
   subleadingJetPt5->SetBinContent(33,2);
   subleadingJetPt5->SetBinContent(35,3);
   subleadingJetPt5->SetBinContent(65,1);
   subleadingJetPt5->SetEntries(3804);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *AText = ptstats->AddText("subleadingJetPt");
   AText->SetTextSize(0.0368);
   AText = ptstats->AddText("Entries = 3804   ");
   AText = ptstats->AddText("Mean  =  43.01");
   AText = ptstats->AddText("RMS   =  34.66");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   subleadingJetPt5->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(subleadingJetPt5);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   subleadingJetPt5->SetLineColor(ci);
   subleadingJetPt5->GetXaxis()->SetTitle("Subleading Jet PT (GeV)");
   subleadingJetPt5->GetXaxis()->SetLabelFont(42);
   subleadingJetPt5->GetXaxis()->SetLabelSize(0.035);
   subleadingJetPt5->GetXaxis()->SetTitleSize(0.035);
   subleadingJetPt5->GetXaxis()->SetTitleFont(42);
   subleadingJetPt5->GetYaxis()->SetTitle("Occurrence");
   subleadingJetPt5->GetYaxis()->SetLabelFont(42);
   subleadingJetPt5->GetYaxis()->SetLabelSize(0.035);
   subleadingJetPt5->GetYaxis()->SetTitleSize(0.035);
   subleadingJetPt5->GetYaxis()->SetTitleFont(42);
   subleadingJetPt5->GetZaxis()->SetLabelFont(42);
   subleadingJetPt5->GetZaxis()->SetLabelSize(0.035);
   subleadingJetPt5->GetZaxis()->SetTitleSize(0.035);
   subleadingJetPt5->GetZaxis()->SetTitleFont(42);
   subleadingJetPt5->Draw("");
   
   TPaveText *pt = new TPaveText(0.3405747,0.9339831,0.6594253,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   AText = pt->AddText("Subleading Jet PT");
   pt->Draw();
   Canvas->Modified();
   Canvas->cd();
   Canvas->SetSelected(Canvas);
}
