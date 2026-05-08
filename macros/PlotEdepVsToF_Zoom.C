void PlotEdepVsToF_Zoom(const char* filename = "outputTof6MeV_AmBe.root",
                        const char* outpng = "edep_vs_tof_zoom.png")
{
  TFile* file = TFile::Open(filename, "READ");
  if (!file || file->IsZombie()) {
    printf("Could not open %s\n", filename);
    return;
  }

  TH2* hist = dynamic_cast<TH2*>(file->Get("EdepVsToF_Tagged"));
  if (!hist) {
    printf("Histogram EdepVsToF_Tagged not found in %s\n", filename);
    file->Close();
    delete file;
    return;
  }

  TH2* h = dynamic_cast<TH2*>(hist->Clone("edep_vs_tof_zoom_clone"));
  h->SetDirectory(0);

  file->Close();
  delete file;

  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas("cEdepVsToFZoom", "Zoomed Edep vs ToF", 1050, 780);
  c->SetGrid();
  c->SetTicks(1, 1);
  c->SetLeftMargin(0.14);
  c->SetBottomMargin(0.12);

  h->SetTitle(";ToF [ns];E dep [keV]");
  h->GetXaxis()->SetRangeUser(0., 500.);
  h->GetYaxis()->SetRangeUser(0., 100.);
  h->GetXaxis()->SetTitleSize(0.048);
  h->GetYaxis()->SetTitleSize(0.048);
  h->GetXaxis()->SetLabelSize(0.038);
  h->GetYaxis()->SetLabelSize(0.038);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->SetMarkerStyle(20);
  h->SetMarkerSize(0.42);
  h->SetMarkerColor(kAzure + 2);
  h->SetLineColor(kAzure + 2);
  h->Draw("SCAT");

  TArrow* hArrow = new TArrow(250., 90., 200., 75., 0.02, "|>");
  hArrow->SetLineColor(kRed + 2);
  hArrow->SetFillColor(kRed + 2);
  hArrow->SetLineWidth(2);
  hArrow->Draw();

  TArrow* xeArrow = new TArrow(210., 42., 55., 19., 0.02, "|>");
  xeArrow->SetLineColor(kRed + 2);
  xeArrow->SetFillColor(kRed + 2);
  xeArrow->SetLineWidth(2);
  xeArrow->Draw();

  TLatex label;
  label.SetTextColor(kRed + 2);
  label.SetTextSize(0.045);
  label.DrawLatex(280., 90., "H recoils");
  label.DrawLatex(220., 43., "Xe recoils");

  c->SaveAs(outpng);
  printf("Saved: %s\n", outpng);
}
