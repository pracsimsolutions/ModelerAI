<?xml version="1.0" encoding="UTF-8"?>
<flexsim-tree version="4" treetype="tree">
<node f="42"><name>ModelerAI</name>
 <node f="40"><name></name></node>
 <node f="42"><name>installdata</name>
  <node f="40"><name></name></node>
  <node f="42" dt="2"><name>add_modules</name><data>VIEW:/modules</data>
   <node f="40"><name></name></node>
   <node f="42" dt="3"><name>ModelerAI</name><data><coupling>null</coupling></data>
    <node f="40"><name></name></node>
    <node f="42" dt="1"><name>rank</name><data>0000000000000000</data></node>
    <node f="42"><name>after</name></node>
    <node f="42" dt="1"><name>into object</name><data>0000000000000000</data></node>
    <node f="42"><name>data</name>
     <node f="40"><name></name></node>
     <node f="42"><name>ModelerAI</name>
      <node f="40"><name></name></node>
      <node f="42" dt="4"><name>ModelerAI</name><data>
       <node f="40"><name>object</name></node>
       <node f="42" dt="3"><name>viewfocus</name><data><coupling>null</coupling></data></node>
       <node f="42" dt="3"><name>objectfocus</name><data><coupling>null</coupling></data></node>
       <node f="42" dt="1"><name>viewwindowtype</name><data>0000000040598000</data>
        <node f="40"><name></name></node>
        <node f="42" dt="2"><name>ModelerAI</name><data>VIEW:/active&gt;Documents/ModelerAI</data>
         <node f="40"><name></name></node></node>
        <node f="42" dt="2"><name>ClickListener</name><data>VIEW:/active&gt;DocListeners/Click</data></node>
       </node>
       <node f="42" dt="2"><name>documentwindow</name><data>ModelerAI</data></node>
       <node f="42"><name>variables</name>
        <node f="40"><name></name></node></node>
       <node f="42"><name>spatial</name>
        <node f="40"><name></name></node>
        <node f="42" dt="1"><name>spatialx</name><data>0000000040900000</data></node>
        <node f="42" dt="1"><name>spatialy</name><data>0000000040590000</data></node>
        <node f="42" dt="1"><name>spatialsx</name><data>00000000407af000</data>
         <node f="40"><name></name></node>
         <node f="42" dt="1"><name>min</name><data>0000000040590000</data></node>
         <node f="42" dt="1"><name>fixed</name><data>000000003ff00000</data></node>
         <node f="42" dt="1"><name>desired</name><data>00000000407af000</data></node>
        </node>
        <node f="42" dt="1"><name>spatialsy</name><data>0000000040830000</data>
         <node f="40"><name></name></node>
         <node f="42" dt="1"><name>min</name><data>0000000040590000</data></node>
         <node f="42" dt="1"><name>fixed</name><data>000000003ff00000</data></node>
         <node f="42" dt="1"><name>desired</name><data>0000000040790000</data></node>
        </node>
       </node>
       <node f="42" dt="2"><name>windowtitle</name><data>ModelerAI</data></node>
       <node f="42" dt="1"><name>tabwindow</name><data>0000000040594000</data>
        <node f="40"><name></name></node>
        <node f="42" dt="2"><name>OnDropWindow</name><data>// dragging from a window to another window
if (objectexists(i)) 
	applicationcommand("dockwindow", i, c, eventcode);
// dragging a tab out of a window
else return applicationcommand("undockwindow", c, 0, dropx(), dropy());</data></node>
       </node>
       <node f="42" dt="2"><name>bitmap</name><data>modules\ModelerAI\ModelerAI.png</data></node>
      </data>
       <node f="40"><name></name></node>
       <node f="42" dt="4"><name>html</name><data>
        <node f="40"><name>object</name></node>
        <node f="42" dt="1"><name>viewwindowtype</name><data>00000000405f0000</data></node>
        <node f="42" dt="1"><name>spatialx</name><data>0000000000000000</data></node>
        <node f="42" dt="1"><name>spatialy</name><data>0000000000000000</data></node>
        <node f="42" dt="1"><name>spatialsx</name><data>00000000405e0000</data></node>
        <node f="42" dt="1"><name>spatialsy</name><data>00000000405e0000</data></node>
        <node f="42" dt="2"><name>coldlink</name><data>..&gt;variables/html</data></node>
        <node f="42" dt="1"><name>alignrightmargin</name><data>0000000000000000</data></node>
        <node f="42" dt="1"><name>alignbottommargin</name><data>0000000000000000</data></node>
        <node f="42"><name>variables</name>
         <node f="40"><name></name></node>
         <node f="42" dt="2"><name>html</name><data>&lt;!DOCTYPE html&gt;
&lt;html class="dark preload"&gt;
&lt;head&gt;
  &lt;meta charset="utf-8" /&gt;
  &lt;meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover"&gt;
  &lt;title&gt;PracSim ModelerAI&lt;/title&gt;
  &lt;script&gt;
    // Anti-flash theme bootstrap. Runs BEFORE any CSS/variables paint so
    // the first frame already has the correct theme — no white-to-dark
    // transition flash on cold open. localStorage is synchronous; OK to
    // read inline here.
    (function () {
      try {
        var saved = localStorage.getItem('modelerai_theme');
        // Valid values: 'dark' (default), 'light', 'system'.
        var theme = (saved === 'light' || saved === 'system') ? saved : 'dark';
        document.documentElement.className = theme + ' preload';
      } catch (e) {
        // Storage unavailable — stay on the default `dark preload` we set in the tag.
      }
      // Drop the .preload class after the first paint so runtime theme
      // switches animate smoothly (the transition is wanted for THEME
      // changes, just not for initial render).
      window.addEventListener('DOMContentLoaded', function () {
        requestAnimationFrame(function () {
          requestAnimationFrame(function () {
            document.documentElement.classList.remove('preload');
          });
        });
      });
    })();
  &lt;/script&gt;
  &lt;script src="flexsimweb/common.js"&gt;&lt;/script&gt;
  &lt;style&gt;
    /* Suppress the 120ms color transition during initial render — otherwise
       the browser's default white body animates into the saved theme on
       first paint and looks like a "light → dark" flash. The .preload class
       is removed by JS on DOMContentLoaded after one animation frame, so
       runtime theme switches still animate smoothly. */
    .preload, .preload *, .preload *::before, .preload *::after {
      transition: none !important;
    }
    /* ============================================================
       Theme — CSS variables. Default is dark; html.light overrides;
       html.system follows prefers-color-scheme. Fallback to dark if
       the OS preference isn't reachable through the CEF panel.
       ============================================================ */
    html, html.dark {
      --bg:            #0b1224;
      --bg-elevated:   #1e293b;
      --bg-input:      #0f172a;
      --bg-sidebar:    #111a32;
      --border:        #334155;
      --border-soft:   #1e293b;
      --text:          #e2e8f0;
      --text-muted:    #94a3b8;
      --text-dim:      #64748b;
      --accent:        #60a5fa;
      --accent-hover:  #93c5fd;
      --accent-fg:     #0b1224;
      --success:       #4ade80;
      --warning:       #facc15;
      --error:         #f87171;
      --error-bg:      #3b1c1c;
    }
    html.light {
      --bg:            #f8fafc;
      --bg-elevated:   #ffffff;
      --bg-input:      #f1f5f9;
      --bg-sidebar:    #f1f5f9;
      --border:        #cbd5e1;
      --border-soft:   #e2e8f0;
      --text:          #0f172a;
      --text-muted:    #475569;
      --text-dim:      #64748b;
      --accent:        #2563eb;
      --accent-hover:  #1d4ed8;
      --accent-fg:     #ffffff;
      --success:       #16a34a;
      --warning:       #ca8a04;
      --error:         #dc2626;
      --error-bg:      #fef2f2;
    }
    @media (prefers-color-scheme: light) {
      html.system {
        --bg:            #f8fafc;
        --bg-elevated:   #ffffff;
        --bg-input:      #f1f5f9;
        --bg-sidebar:    #f1f5f9;
        --border:        #cbd5e1;
        --border-soft:   #e2e8f0;
        --text:          #0f172a;
        --text-muted:    #475569;
        --text-dim:      #64748b;
        --accent:        #2563eb;
        --accent-hover:  #1d4ed8;
        --accent-fg:     #ffffff;
        --success:       #16a34a;
        --warning:       #ca8a04;
        --error:         #dc2626;
        --error-bg:      #fef2f2;
      }
    }

    *, *::before, *::after { box-sizing: border-box; }
    html, body {
      height: 100%; margin: 0;
      background: var(--bg); color: var(--text);
      font: 14px/1.5 -apple-system, "Segoe UI", system-ui, sans-serif;
      /* Smooth theme swap — colors transition rather than snap. */
      transition: background-color 120ms ease, color 120ms ease;
    }
    body { display: flex; flex-direction: column; }
    button, select, input, textarea {
      transition: background-color 120ms ease, border-color 120ms ease, color 120ms ease;
    }
    .bubble, .stat, .placeholder, .preview-box, .placeholder-block {
      transition: background-color 120ms ease, border-color 120ms ease, color 120ms ease;
    }

    /* ----- Custom scrollbars (Chromium/WebKit pseudo-elements; CEF
       is Chromium-based so these apply everywhere). Replaces the
       OS-native bar that clashes with the theme. Thin, rounded, fades
       in on hover, theme-aware via the --scroll-* variables we define
       in :root / dark-mode block. ----- */
    :root {
      --scroll-track:       transparent;
      --scroll-thumb:       rgba(0, 0, 0, 0.22);
      --scroll-thumb-hover: rgba(0, 0, 0, 0.40);
    }
    @media (prefers-color-scheme: dark) {
      :root {
        --scroll-thumb:       rgba(255, 255, 255, 0.16);
        --scroll-thumb-hover: rgba(255, 255, 255, 0.32);
      }
    }
    /* Apply to every scrollable element. */
    * {
      scrollbar-width: thin;                                  /* Firefox fallback */
      scrollbar-color: var(--scroll-thumb) var(--scroll-track);
    }
    *::-webkit-scrollbar {
      width: 10px; height: 10px;
    }
    *::-webkit-scrollbar-track {
      background: var(--scroll-track);
      border-radius: 8px;
    }
    *::-webkit-scrollbar-thumb {
      background: var(--scroll-thumb);
      border-radius: 8px;
      /* Padding-like effect — 2px inset so the thumb looks thinner
         than the track gutter; gives a softer, more modern feel. */
      border: 2px solid transparent;
      background-clip: padding-box;
    }
    *::-webkit-scrollbar-thumb:hover {
      background: var(--scroll-thumb-hover);
      background-clip: padding-box;
    }
    *::-webkit-scrollbar-corner {
      background: transparent;
    }
    /* Thin variant for tight UI elements (popovers, picker lists). */
    .scroll-thin::-webkit-scrollbar {
      width: 6px; height: 6px;
    }
    .scroll-thin::-webkit-scrollbar-thumb {
      border-width: 1px;
    }

    /* ----- tabs ----- */
    #tabbar {
      display: flex;
      border-bottom: 1px solid var(--border);
      background: var(--bg-elevated);
      flex-shrink: 0;
    }
    .tab {
      padding: 10px 18px;
      cursor: pointer;
      color: var(--text-muted);
      border-bottom: 2px solid transparent;
      font-weight: 600;
      font-size: 13px;
      user-select: none;
    }
    .tab:hover { color: var(--text); }
    .tab.active {
      color: var(--text);
      border-bottom-color: var(--accent);
    }
    .pane { display: none; flex: 1; min-height: 0; }
    .pane.active { display: flex; }

    /* Remote-clients badge in the tabbar. Pushed to the far right via
       margin-left:auto (works because #tabbar is display:flex). */
    .remote-badge {
      display: inline-flex; align-items: center;
      margin-left: auto;
      margin-right: 10px;
      padding: 4px 8px;
      background: rgba(74, 144, 226, 0.15);
      border: 1px solid rgba(74, 144, 226, 0.4);
      border-radius: 999px;
      font-size: 12px; color: #4a90e2;
      cursor: pointer;
      white-space: nowrap;
      align-self: center;
    }

    /* ============================================================
       Chat pane
       ============================================================ */
    #pane-chat { flex-direction: column; }

    #chat-header {
      display: flex; align-items: center; gap: 12px;
      padding: 8px 14px;
      border-bottom: 1px solid var(--border-soft);
      background: var(--bg-elevated);
      flex-shrink: 0;
      font-size: 12px;
    }
    .model-pill {
      display: inline-flex; align-items: center; gap: 6px;
      padding: 4px 10px; border-radius: 999px;
      background: var(--bg-input);
      border: 1px solid var(--border);
      color: var(--text);
      font-size: 12px; font-weight: 600;
      cursor: pointer;            /* click to open the picker */
      user-select: none;
    }
    .model-pill:hover { border-color: var(--accent); }
    .model-pill.open  { border-color: var(--accent); }
    .model-pill .dot {
      width: 6px; height: 6px; border-radius: 50%;
      background: var(--success);
    }
    .model-pill.unauth .dot { background: var(--text-dim); }
    .model-pill-caret {
      font-size: 9px; color: var(--text-muted);
      margin-left: 2px;
    }
    /* Floating model picker — drops from the model pill, lists every
       known model across providers, click to switch. */
    #model-picker-popover {
      position: absolute;
      display: none;
      z-index: 90;
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      border-radius: 8px;
      box-shadow: 0 8px 24px rgba(0,0,0,0.25);
      min-width: 280px; max-width: 360px;
      max-height: 60vh; overflow-y: auto;
      padding: 4px 0;
      font-size: 12.5px;
    }
    #model-picker-popover.open { display: block; }
    .mp-group-label {
      padding: 6px 12px 4px;
      font-size: 10px; font-weight: 700; text-transform: uppercase;
      letter-spacing: 0.4px;
      color: var(--text-muted);
    }
    .mp-item {
      display: flex; align-items: center; gap: 8px;
      padding: 6px 12px;
      cursor: pointer;
      color: var(--text);
    }
    .mp-item:hover { background: var(--bg-input); }
    .mp-item.active { background: var(--bg-input); }
    .mp-item .mp-check {
      width: 14px; color: var(--accent); font-weight: 700;
    }
    .mp-item .mp-name { flex: 1; font-weight: 600; }
    .mp-item .mp-cost {
      font-size: 10px; color: var(--text-muted);
      font-family: ui-monospace, Consolas, monospace;
    }
    .trust-pill {
      display: inline-flex; align-items: center; gap: 6px;
      padding: 3px 9px; border-radius: 12px;
      font-size: 11px; font-weight: 600;
      cursor: pointer;
      margin-left: 8px;
    }
    .trust-pill.trust-green {
      background: rgba(58, 175, 95, 0.18);
      color: #2d8c4a;
      border: 1px solid rgba(58, 175, 95, 0.45);
    }
    .trust-pill.trust-amber {
      background: rgba(217, 158, 53, 0.18);
      color: #b07a16;
      border: 1px solid rgba(217, 158, 53, 0.50);
    }
    .trust-pill.trust-red {
      background: rgba(192, 57, 43, 0.20);
      color: #c0392b;
      border: 1px solid rgba(192, 57, 43, 0.55);
    }
    .trust-pill.trust-local {
      background: rgba(80, 130, 255, 0.16);
      color: #3257c8;
      border: 1px solid rgba(80, 130, 255, 0.45);
    }
    .trust-pill.trust-unknown {
      background: var(--bg-input);
      color: var(--text-muted);
      border: 1px solid var(--border);
    }

    /* (Free-tier quota pill removed — product is freeware.) */

    /* ----- privacy panel ----- */
    .priv-table {
      width: 100%; border-collapse: collapse;
      margin: 8px 0 16px;
      font-size: 12px;
    }
    .priv-table th {
      text-align: left;
      padding: 6px 8px;
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      font-weight: 700;
    }
    .priv-table td {
      padding: 8px;
      border: 1px solid var(--border-soft);
      vertical-align: top;
    }
    .priv-row.priv-green td { background: rgba(58, 175, 95, 0.06); }
    .priv-row.priv-amber td { background: rgba(217, 158, 53, 0.10); }
    .priv-row.priv-red td   { background: rgba(192, 57, 43, 0.12); }
    .priv-bullets { margin: 6px 0 16px; padding-left: 22px; font-size: 12px; line-height: 1.55; }
    .priv-bullets li { margin: 4px 0; }

    /* ----- onboarding wizard ----- */
    #onb-overlay {
      position: fixed; inset: 0;
      background: rgba(0,0,0,0.7);
      display: none; align-items: center; justify-content: center;
      z-index: 150;
      padding: 20px;
    }
    #onb-overlay.shown { display: flex; }
    .onb-card {
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      border-radius: 10px;
      max-width: 720px; width: 100%;
      max-height: 90vh;
      padding: 22px 26px;
      box-shadow: 0 12px 50px rgba(0,0,0,0.55);
      display: flex; flex-direction: column;
    }
    .onb-stepper {
      display: flex; gap: 4px; flex-wrap: wrap;
      margin-bottom: 14px;
      font-size: 11px;
    }
    .onb-step {
      padding: 4px 10px;
      border-radius: 12px;
      background: var(--bg-input);
      color: var(--text-muted);
    }
    .onb-step.active {
      background: var(--accent); color: var(--accent-fg);
      font-weight: 700;
    }
    .onb-step.done { color: var(--text); background: var(--bg-sidebar); }
    .onb-pane {
      overflow-y: auto;
      flex: 1;
      font-size: 13px; line-height: 1.55;
    }
    .onb-pane h2 { margin: 4px 0 10px; font-size: 16px; font-weight: 700; }
    .onb-pane p, .onb-pane ul, .onb-pane ol { margin: 0 0 10px; }
    .onb-pane ul, .onb-pane ol { padding-left: 22px; }
    .onb-pane code {
      background: var(--bg-input);
      padding: 1px 5px; border-radius: 4px;
      font: 12px monospace;
    }
    .onb-keyrow {
      display: flex; gap: 8px; margin: 12px 0;
    }
    .onb-keyrow input {
      flex: 1; padding: 8px 10px;
      font: 12px monospace;
    }
    .onb-keyrow button {
      padding: 8px 16px; border: 0; border-radius: 6px;
      background: var(--accent); color: var(--accent-fg);
      font-weight: 600; cursor: pointer;
    }
    .onb-key-status { margin-top: 4px; }
    .onb-key-status.ok    { color: #2d8c4a; }
    .onb-key-status.err   { color: #c0392b; }
    .onb-ack {
      display: flex; align-items: flex-start; gap: 8px;
      margin: 10px 0;
      padding: 8px 12px;
      background: var(--bg-input);
      border-radius: 6px;
      font-size: 12px;
    }
    .onb-ack input { margin-top: 3px; }
    .onb-model-grid {
      display: grid; gap: 10px;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      margin: 10px 0;
    }
    .onb-model-card {
      background: var(--bg);
      border: 1px solid var(--border);
      border-radius: 8px;
      padding: 12px;
    }
    .onb-model-card-recommended {
      border: 2px solid var(--accent);
    }
    .onb-model-card h4 { margin: 0 0 4px; font-size: 13px; font-weight: 700; }
    .onb-model-card .meta { font-size: 11px; }
    .onb-model-cost {
      font: 10px monospace; margin: 6px 0;
      color: var(--text-muted);
    }
    .onb-pick {
      width: 100%;
      padding: 6px 10px; border: 0; border-radius: 6px;
      background: var(--bg-input); color: var(--text);
      cursor: pointer; font-size: 12px; font-weight: 600;
    }
    .onb-pick.picked {
      background: var(--accent); color: var(--accent-fg);
    }
    .onb-actions {
      display: flex; justify-content: space-between; align-items: center;
      gap: 10px; margin-top: 14px;
      padding-top: 14px;
      border-top: 1px solid var(--border-soft);
    }
    .onb-actions button {
      padding: 8px 16px; border: 0; border-radius: 6px;
      font-weight: 600; cursor: pointer; font-size: 13px;
    }
    .onb-skip {
      background: transparent; color: var(--text-muted);
      margin-right: auto;
    }
    .onb-back {
      background: var(--bg); color: var(--text);
      border: 1px solid var(--border) !important;
    }
    .onb-back:disabled { opacity: 0.35; cursor: not-allowed; }
    .onb-next, .onb-finish {
      background: var(--accent); color: var(--accent-fg);
    }

    /* ----- custom-provider acknowledgement modal ----- */
    #cp-ack-overlay {
      position: fixed; inset: 0;
      background: rgba(0,0,0,0.7);
      display: none; align-items: center; justify-content: center;
      z-index: 180;
      padding: 20px;
    }
    #cp-ack-overlay.shown { display: flex; }
    .cp-ack-card {
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      border-left: 4px solid #d18b00;
      border-radius: 10px;
      max-width: 620px; width: 100%;
      max-height: 85vh;
      padding: 20px 22px;
      box-shadow: 0 12px 50px rgba(0,0,0,0.55);
      display: flex; flex-direction: column;
    }
    .cp-ack-card.cp-ack-red { border-left-color: #c0392b; }
    .cp-ack-card h2 { margin: 0 0 10px; font-size: 15px; font-weight: 700; }
    .cp-ack-card.cp-ack-red h2 { color: #d44638; }
    .cp-ack-body {
      overflow-y: auto;
      font-size: 12px; line-height: 1.55;
      flex: 1;
    }
    .cp-ack-body p { margin: 0 0 8px; }
    .cp-ack-body ul { margin: 0 0 8px; padding-left: 22px; }
    .cp-ack-body code {
      background: var(--bg-input);
      padding: 1px 5px; border-radius: 4px;
      font: 11px monospace;
    }
    .cp-ack-confirm-row {
      display: flex; align-items: flex-start; gap: 8px;
      margin: 10px 0 12px;
      font-size: 12px;
      padding: 8px 10px;
      background: var(--bg-input);
      border-radius: 6px;
    }
    .cp-ack-confirm-row input { margin-top: 3px; }
    .cp-ack-actions {
      display: flex; justify-content: flex-end; gap: 10px;
    }
    .cp-ack-actions button {
      padding: 8px 14px;
      border: 0; border-radius: 6px;
      font-weight: 600; cursor: pointer;
      font-size: 12px;
    }
    .cp-ack-cancel {
      background: var(--bg); color: var(--text-muted);
      border: 1px solid var(--border) !important;
    }
    .cp-ack-agree {
      background: var(--accent); color: var(--accent-fg);
    }
    .cp-ack-agree:disabled { opacity: 0.45; cursor: not-allowed; }
    .cp-ack-red .cp-ack-agree { background: #c0392b; }

    /* ----- approval modal (mutating tool ack) ----- */
    #approval-overlay {
      position: absolute;
      inset: 0;
      background: rgba(0,0,0,0.55);
      display: none; align-items: center; justify-content: center;
      z-index: 100;
      padding: 20px;
    }
    #approval-overlay.shown { display: flex; }
    .approval-card {
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      border-left: 4px solid var(--warning);
      border-radius: 10px;
      max-width: 520px; width: 100%;
      padding: 18px 20px;
      box-shadow: 0 10px 40px rgba(0,0,0,0.5);
    }
    .approval-card h3 {
      margin: 0 0 4px; font-size: 14px; font-weight: 700;
    }
    .approval-card .approval-cat {
      display: inline-block;
      padding: 2px 8px; border-radius: 4px;
      background: var(--warning);
      color: #000; font-size: 10px; font-weight: 700;
      text-transform: uppercase;
      margin-bottom: 12px;
    }
    .approval-card .approval-desc {
      color: var(--text-muted); font-size: 13px;
      margin-bottom: 10px;
    }
    .approval-card .approval-args {
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      border-radius: 5px;
      padding: 8px 10px;
      font: 12px ui-monospace, Consolas, monospace;
      color: var(--text);
      white-space: pre-wrap; word-wrap: break-word;
      max-height: 180px; overflow-y: auto;
      margin-bottom: 14px;
    }
    .approval-card .approval-actions {
      display: flex; gap: 8px; justify-content: flex-end; flex-wrap: wrap;
    }
    .approval-card button {
      padding: 7px 14px;
      border: 1px solid var(--border);
      background: var(--bg-input);
      color: var(--text);
      border-radius: 5px;
      cursor: pointer;
      font: inherit; font-size: 13px; font-weight: 600;
    }
    .approval-card button.deny    { color: var(--error); border-color: var(--error); }
    .approval-card button.deny:hover    { background: var(--error); color: #fff; }
    .approval-card button.approve { background: var(--accent); color: var(--accent-fg); border-color: var(--accent); }
    .approval-card button.approve:hover { background: var(--accent-hover); }
    .approval-card button.always  { color: var(--success); border-color: var(--success); }
    .approval-card button.always:hover  { background: var(--success); color: #000; }

    /* ----- inline question bubble (ask_user_question) ----- */
    .bubble-question {
      background: var(--bg-elevated);
      border: 1px solid var(--accent);
      border-left: 4px solid var(--accent);
      color: var(--text);
      align-self: flex-start;
      max-width: 95%;
    }
    .bubble-question .q-head {
      font-weight: 700; margin-bottom: 4px;
      display: flex; align-items: center; gap: 6px;
    }
    .bubble-question .q-head::before {
      content: "❓"; font-size: 14px;
    }
    .bubble-question .q-text {
      margin-bottom: 10px; line-height: 1.5;
    }
    .bubble-question .q-options {
      display: flex; flex-direction: column; gap: 6px;
    }
    .bubble-question .q-option {
      text-align: left;
      padding: 8px 12px;
      border: 1px solid var(--border);
      background: var(--bg-input); color: var(--text);
      border-radius: 6px;
      cursor: pointer;
      font: inherit; font-size: 13px;
    }
    .bubble-question .q-option:hover {
      border-color: var(--accent); color: var(--accent);
    }
    .bubble-question .q-option .q-option-desc {
      display: block;
      color: var(--text-muted); font-size: 11px;
      margin-top: 2px;
    }
    .bubble-question .q-freetext-row {
      display: flex; gap: 6px; margin-top: 4px;
    }
    .bubble-question .q-freetext-row input {
      flex: 1; padding: 6px 8px;
      background: var(--bg-input); color: var(--text);
      border: 1px solid var(--border); border-radius: 5px;
      font: inherit;
    }
    .bubble-question.answered {
      border-color: var(--success); border-left-color: var(--success);
      opacity: 0.85;
    }
    .bubble-question .q-answered-marker {
      color: var(--success); font-weight: 700; font-size: 12px;
      margin-top: 6px;
    }

    /* (Upgrade banner removed — product is freeware.) */
    .mode-switch {
      margin-left: auto;
      display: inline-flex; border: 1px solid var(--border); border-radius: 6px;
      overflow: hidden;
    }
    .mode-btn {
      padding: 4px 10px; cursor: pointer;
      background: transparent; color: var(--text-muted);
      border: 0;
      font: inherit; font-size: 12px; font-weight: 600;
    }
    .mode-btn.active {
      background: var(--accent); color: var(--accent-fg);
    }
    .mode-btn:disabled {
      cursor: help; opacity: 0.45;
      background: repeating-linear-gradient(
        45deg,
        transparent, transparent 4px,
        var(--border-soft) 4px, var(--border-soft) 5px
      );
    }
    .mode-btn:disabled:hover { color: var(--text-muted); }

    #messages {
      /* `overflow-y: scroll` (not `auto`) so the scrollbar gutter is
         always reserved — gives a visible affordance that there's
         history to scroll through even when the messages currently
         fit. The custom *::-webkit-scrollbar styling above paints it
         to match the theme. */
      flex: 1; overflow-y: scroll; padding: 16px;
      display: flex; flex-direction: column; gap: 10px;
    }
    .bubble {
      padding: 10px 14px;
      border-radius: 12px;
      white-space: pre-wrap;
      max-width: 85%;
      word-wrap: break-word;
      position: relative;
    }
    .bubble-content { /* contents wrap so the actions toolbar can sit beside */ }
    .bubble-actions {
      display: flex; gap: 4px;
      margin-top: 6px;
      opacity: 0.55;            /* visible always, just quiet */
      transition: opacity 120ms ease-in-out;
      /* Selecting the bubble's text shouldn't pick up the action-bar text
         ("Copy", "Regenerate") — when the user does a select-all-and-copy
         on the bubble, those button labels would end up in their clipboard
         as trailing words. user-select:none keeps them out of selections. */
      user-select: none;
      -webkit-user-select: none;
    }
    .bubble:hover .bubble-actions { opacity: 1; }
    .bubble-action {
      font-size: 11px;
      padding: 2px 8px;
      border: 1px solid var(--border-soft);
      background: transparent;
      color: var(--text-muted);
      border-radius: 4px;
      cursor: pointer;
    }
    .bubble-action:hover {
      color: var(--text);
      border-color: var(--border);
      background: var(--bg);
    }
    /* On the user bubble (blue accent background), --text-muted and the
       default border-soft are nearly invisible. Use accent-fg with a
       translucent white border so the Copy button reads. */
    .bubble-user .bubble-action {
      color: var(--accent-fg);
      border-color: rgba(255, 255, 255, 0.35);
    }
    .bubble-user .bubble-action:hover {
      color: var(--accent-fg);
      background: rgba(255, 255, 255, 0.12);
      border-color: rgba(255, 255, 255, 0.55);
    }
    .bubble-user {
      background: var(--accent);
      color: var(--accent-fg);
      align-self: flex-end;
    }
    /* Fixed top-right build version marker. Bumped by .0000001 on every
       source change so smoke tests can verify "am I running the new code?"
       at a glance. Visible in both host and phone mode (not nested in
       #tabbar or #pane-settings). */
    #app-version {
      position: fixed;
      top: 4px;
      right: 8px;
      z-index: 9999;
      font: 10px/1 ui-monospace, Consolas, "Cascadia Mono", monospace;
      color: var(--text-muted);
      opacity: 0.6;
      pointer-events: none;     /* clicks pass through to underlying UI */
      user-select: text;        /* but the version can still be copied */
      letter-spacing: 0.3px;
    }

    /* Author attribution for shared-chat user_message_broadcast bubbles.
       Sits above the message text; smaller + translucent so it doesn't
       compete with the message content. */
    .bubble-author {
      display: block;
      font-size: 10px;
      font-weight: 600;
      letter-spacing: 0.3px;
      opacity: 0.65;
      margin-bottom: 3px;
    }
    .bubble-assistant {
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      align-self: flex-start;
    }
    /* Slash-command output renders here. Visually distinct from real
       LLM replies so it's clear the response is local + free. */
    .bubble-system {
      background: var(--bg-input);
      border: 1px dashed var(--border);
      color: var(--text-muted);
      align-self: flex-start;
      font: 12.5px/1.5 ui-monospace, Consolas, "Cascadia Mono", monospace;
      max-width: 100%;
    }
    .bubble-system::before {
      content: "⌘";
      display: inline-block;
      margin-right: 8px;
      color: var(--accent);
      font-weight: 700;
      font-family: -apple-system, "Segoe UI", system-ui, sans-serif;
    }
    /* Markdown rendering inside assistant + system bubbles. Block tags
       supersede the bubble's pre-wrap so paragraphs / lists / code blocks
       look natural; inline elements (code, strong, em) inherit color. */
    .bubble-assistant, .bubble-system { white-space: normal; }
    .bubble-assistant p, .bubble-system p { margin: 0 0 8px; }
    .bubble-assistant p:last-child, .bubble-system p:last-child { margin-bottom: 0; }
    .bubble-assistant h2, .bubble-assistant h3, .bubble-assistant h4 {
      margin: 12px 0 6px; line-height: 1.3;
    }
    .bubble-assistant h2 { font-size: 16px; font-weight: 700; }
    .bubble-assistant h3 { font-size: 14px; font-weight: 700; }
    .bubble-assistant h4 { font-size: 13px; font-weight: 600; }
    .bubble-assistant ul, .bubble-system ul {
      margin: 0 0 8px; padding-left: 22px;
    }
    .bubble-assistant li, .bubble-system li { margin: 2px 0; }
    .bubble-assistant code, .bubble-system code {
      background: var(--bg-input);
      padding: 1px 5px; border-radius: 4px;
      font: 12px/1.4 ui-monospace, Consolas, "Cascadia Mono", monospace;
    }
    .bubble-assistant pre, .bubble-system pre {
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      border-radius: 6px;
      padding: 10px 12px;
      overflow-x: auto;
      margin: 8px 0;
    }
    .bubble-assistant pre code, .bubble-system pre code {
      background: transparent; padding: 0;
      font-size: 12px;
    }
    .bubble-assistant a, .bubble-system a {
      color: var(--accent); text-decoration: underline;
    }
    .bubble-assistant ol, .bubble-system ol {
      margin: 0 0 8px; padding-left: 22px;
    }
    .bubble-assistant blockquote, .bubble-system blockquote {
      margin: 8px 0;
      padding: 6px 12px;
      border-left: 3px solid var(--border);
      color: var(--text-muted);
      background: var(--bg-input);
      border-radius: 0 4px 4px 0;
    }
    .bubble-assistant table.md-table, .bubble-system table.md-table {
      border-collapse: collapse;
      margin: 8px 0;
      font-size: 12px;
    }
    .bubble-assistant table.md-table th,
    .bubble-assistant table.md-table td,
    .bubble-system table.md-table th,
    .bubble-system table.md-table td {
      border: 1px solid var(--border-soft);
      padding: 4px 8px;
      text-align: left;
      vertical-align: top;
    }
    .bubble-assistant table.md-table th,
    .bubble-system table.md-table th {
      background: var(--bg-input);
      font-weight: 700;
    }
    /* Tool-call batch — collapses consecutive tool calls within a turn
       into one compact element. Summary line shows per-tool counts;
       expand to see individual calls with args + results. */
    .bubble-tool-batch {
      background: var(--bg-input);
      border: 1px solid var(--border);
      color: var(--text);
      align-self: flex-start;
      max-width: 95%;
      font-size: 12.5px;
    }
    .tool-batch-summary {
      display: flex; align-items: center; gap: 8px;
      font-family: -apple-system, "Segoe UI", system-ui, sans-serif;
    }
    .tool-batch-summary::before {
      content: "🔧";
    }
    .tool-batch-counts {
      color: var(--text);
      font-size: 12.5px;
    }
    .tool-batch-details {
      margin-top: 6px;
    }
    .tool-batch-details &gt; summary {
      cursor: pointer;
      color: var(--text-muted);
      font-size: 11px;
      list-style: none;
      user-select: none;
    }
    .tool-batch-details &gt; summary::before {
      content: "▸ ";
    }
    .tool-batch-details[open] &gt; summary::before {
      content: "▾ ";
    }
    .tool-batch-list {
      margin-top: 6px;
      display: flex; flex-direction: column; gap: 4px;
      padding-left: 4px;
      border-left: 2px solid var(--border-soft);
    }
    .tool-batch-item {
      padding: 4px 8px;
      background: var(--bg);
      border-radius: 4px;
      font-family: ui-monospace, Consolas, "Cascadia Mono", monospace;
      font-size: 11px;
    }
    .tool-batch-item .tool-item-head {
      display: flex; align-items: baseline; gap: 6px;
    }
    .tool-batch-item .tool-item-head strong {
      font-family: -apple-system, "Segoe UI", system-ui, sans-serif;
      font-size: 12px;
    }
    .tool-batch-item .tool-item-status {
      margin-left: auto;
      font-size: 12px;
      color: var(--text-muted);
    }
    .tool-batch-item.ok   .tool-item-status { color: var(--success); }
    .tool-batch-item.fail .tool-item-status { color: var(--error); }
    .tool-batch-item .tool-item-args,
    .tool-batch-item .tool-item-result {
      color: var(--text-muted);
      white-space: pre-wrap;
      word-wrap: break-word;
      line-height: 1.4;
      margin-top: 2px;
    }
    .tool-batch-item .tool-item-result { color: var(--text); }
    /* Click-to-expand for tool args/result fields. Collapsed shows the
       short summary on one line with an ellipsis; expanded shows the full
       JSON in a scrollable box. */
    .tool-field-expandable {
      cursor: pointer;
      position: relative;
    }
    .tool-field-expandable:not(.expanded) {
      white-space: nowrap;
      overflow: hidden;
      text-overflow: ellipsis;
    }
    .tool-field-expandable:hover {
      background: rgba(127, 127, 127, 0.08);
    }
    .tool-field-expandable.expanded {
      max-height: 400px;
      overflow-y: auto;
      background: rgba(127, 127, 127, 0.05);
      padding: 4px 6px;
      border-radius: 4px;
      font: 12px/1.45 ui-monospace, Consolas, "Cascadia Mono", monospace;
    }
    .bubble-error {
      background: var(--error-bg);
      border: 1px solid var(--error);
      color: var(--error);
      align-self: stretch;
      max-width: 100%;
      font-size: 13px;
    }
    .placeholder {
      color: var(--text-muted);
      text-align: center;
      padding: 28px 20px;
      font-size: 13px;
      line-height: 1.7;
    }
    .placeholder strong { color: var(--text); display: block; margin-bottom: 6px; }
    .placeholder code {
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      border-radius: 3px;
      padding: 1px 6px;
      font: 12px ui-monospace, Consolas, monospace;
    }
    #composer-wrap {
      position: relative;
      flex-shrink: 0;
    }
    /* Composer layout. Two rows stacked vertically:
       1. The textarea — grows in height when the user drags the splitter.
       2. The action toolbar — FIXED HEIGHT regardless of textarea size, so the
          send button doesn't get pushed around as the input grows.
       Pattern lifted from VS Code's chat input UI. */
    #composer {
      display: flex;
      flex-direction: column;
      gap: 6px;
      padding: 10px 12px 12px;
      border-top: 1px solid var(--border);
      background: var(--bg-elevated);
    }
    #composer-actions {
      display: flex;
      align-items: center;
      gap: 6px;
      flex-shrink: 0;
      height: 34px;             /* fixed — does NOT grow with textarea */
    }
    #composer-actions-left {
      display: flex;
      align-items: center;
      gap: 4px;
      flex: 1 1 auto;            /* eats horizontal slack so right side pins */
      min-width: 0;
    }
    #composer-actions-right {
      display: flex;
      align-items: center;
      gap: 6px;
      flex-shrink: 0;
    }
    .composer-action-btn {
      width: 28px; height: 28px;
      display: inline-flex; align-items: center; justify-content: center;
      background: transparent;
      color: var(--text-muted);
      border: 1px solid transparent;
      border-radius: 5px;
      cursor: pointer;
      padding: 0;
      transition: background 80ms ease, color 80ms ease, border-color 80ms ease;
    }
    .composer-action-btn:hover {
      background: var(--bg-input);
      color: var(--text);
      border-color: var(--border-soft);
    }
    .composer-action-btn:disabled {
      opacity: 0.4;
      cursor: not-allowed;
    }
    .composer-action-btn svg {
      width: 16px; height: 16px;
      stroke: currentColor;
      fill: none;
      stroke-width: 2;
      stroke-linecap: round;
      stroke-linejoin: round;
    }

    /* Wider toolbar buttons used for the model + mode pickers. Show a short
       label + a caret; clicking opens a popover anchored above the button. */
    .composer-toolbar-btn {
      display: inline-flex;
      align-items: center;
      gap: 4px;
      height: 28px;
      max-width: 200px;
      padding: 0 8px;
      background: transparent;
      color: var(--text-muted);
      border: 1px solid transparent;
      border-radius: 5px;
      cursor: pointer;
      font: inherit;
      font-size: 12px;
      transition: background 80ms ease, color 80ms ease, border-color 80ms ease;
      white-space: nowrap;
      overflow: hidden;
    }
    .composer-toolbar-btn:hover,
    .composer-toolbar-btn.open {
      background: var(--bg-input);
      color: var(--text);
      border-color: var(--border-soft);
    }
    .composer-toolbar-btn .tb-icon {
      flex-shrink: 0;
      width: 14px; height: 14px;
      display: inline-flex; align-items: center; justify-content: center;
    }
    .composer-toolbar-btn .tb-icon svg {
      width: 14px; height: 14px;
      stroke: currentColor; fill: none;
      stroke-width: 2; stroke-linecap: round; stroke-linejoin: round;
    }
    .composer-toolbar-btn .tb-label {
      flex: 1 1 auto;
      overflow: hidden;
      text-overflow: ellipsis;
      font-weight: 600;
    }
    .composer-toolbar-btn .tb-caret {
      flex-shrink: 0;
      font-size: 9px;
      opacity: 0.7;
    }

    /* Attachment chip row — lives ABOVE the textarea, hidden when no
       attachments are present. Chips render as compact rounded tiles with
       a filename / dimensions and an X to remove. Matches VS Code's chat
       attachment UI. */
    #attachments-row {
      display: flex;
      flex-wrap: wrap;
      gap: 6px;
      padding: 0 0 6px 0;
    }
    #attachments-row:empty { display: none; }
    .attach-chip {
      display: inline-flex; align-items: center; gap: 6px;
      max-width: 280px;
      padding: 3px 6px 3px 8px;
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      border-radius: 6px;
      font-size: 11px;
      color: var(--text);
    }
    .attach-chip .chip-thumb {
      width: 18px; height: 18px;
      border-radius: 3px;
      object-fit: cover;
      flex-shrink: 0;
      background: var(--bg-elevated);
    }
    .attach-chip .chip-name {
      font-weight: 600;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
      max-width: 140px;
    }
    .attach-chip .chip-dim {
      color: var(--text-muted);
      font-family: ui-monospace, Consolas, monospace;
      font-size: 10px;
    }
    .attach-chip .chip-remove {
      background: transparent;
      border: 0;
      color: var(--text-muted);
      font: inherit; font-size: 14px;
      cursor: pointer;
      padding: 0 2px;
      line-height: 1;
      border-radius: 3px;
    }
    .attach-chip .chip-remove:hover { background: var(--border); color: var(--text); }

    /* Drag-over visual: pulses the composer outline when the user drags a
       file onto it from outside the window. */
    #composer.drag-over {
      box-shadow: inset 0 0 0 2px var(--accent);
      background: rgba(80, 130, 255, 0.06);
    }

    /* In-message thumbnail strip: shown under the user bubble for any
       attached images so the user can see what they sent. Click to open
       at native size. */
    .bubble-attachments {
      display: flex; flex-wrap: wrap; gap: 6px;
      margin-top: 8px;
    }
    .bubble-attachment {
      max-width: 200px;
      max-height: 140px;
      border-radius: 6px;
      border: 1px solid var(--border-soft);
      cursor: zoom-in;
      object-fit: cover;
      background: var(--bg-elevated);
      transition: border-color 80ms ease, transform 80ms ease;
    }
    .bubble-attachment:hover {
      border-color: var(--accent);
      transform: scale(1.01);
    }

    /* Toast-style ephemeral warning shown when attachment size exceeds
       Anthropic's recommended ceiling (5 MB encoded). Auto-dismisses. */
    #composer-toast {
      position: absolute;
      left: 12px; right: 12px;
      bottom: 100%;
      margin-bottom: 6px;
      padding: 8px 12px;
      background: var(--bg-elevated);
      border: 1px solid var(--accent);
      border-radius: 6px;
      color: var(--text);
      font-size: 12px;
      box-shadow: 0 4px 16px rgba(0,0,0,0.3);
      pointer-events: none;
      opacity: 0;
      transform: translateY(4px);
      transition: opacity 160ms ease, transform 160ms ease;
      z-index: 60;
    }
    #composer-toast.show {
      opacity: 1;
      transform: translateY(0);
    }

    /* Mode popover — opens above the Mode toolbar button. Vertical list of
       modes with name + description + check on current. Effort selector at
       the bottom row. Matches VS Code's "Modes" dropdown. */
    #mode-popover {
      position: absolute;
      display: none;
      z-index: 95;
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      border-radius: 8px;
      box-shadow: 0 8px 24px rgba(0,0,0,0.25);
      min-width: 320px; max-width: 360px;
      padding: 0;
      font-size: 12.5px;
    }
    #mode-popover.open { display: block; }
    .mode-pop-header {
      display: flex; align-items: center; gap: 8px;
      padding: 8px 12px;
      border-bottom: 1px solid var(--border-soft);
      color: var(--text-muted);
      font-size: 11px;
      font-weight: 600;
      text-transform: uppercase;
      letter-spacing: 0.4px;
    }
    .mode-pop-header .mp-hint {
      margin-left: auto;
      font-weight: 400;
      text-transform: none;
      letter-spacing: 0;
      font-size: 11px;
    }
    .mode-pop-row {
      display: flex; align-items: flex-start; gap: 10px;
      padding: 8px 12px;
      cursor: pointer;
      color: var(--text);
      border-bottom: 1px solid var(--border-soft);
      transition: background 80ms ease;
    }
    .mode-pop-row:last-child { border-bottom: 0; }
    .mode-pop-row:hover { background: var(--bg-input); }
    .mode-pop-row.selected { background: rgba(80,130,255,0.10); }
    .mode-pop-row .mp-icon {
      flex-shrink: 0;
      width: 18px; height: 18px;
      margin-top: 1px;
      color: var(--text-muted);
    }
    .mode-pop-row .mp-icon svg {
      width: 16px; height: 16px;
      stroke: currentColor; fill: none;
      stroke-width: 2; stroke-linecap: round; stroke-linejoin: round;
    }
    .mode-pop-row .mp-body { flex: 1 1 auto; min-width: 0; }
    .mode-pop-row .mp-title {
      font-weight: 600; font-size: 13px;
      color: var(--text);
    }
    .mode-pop-row .mp-desc {
      color: var(--text-muted);
      font-size: 11.5px;
      line-height: 1.4;
      margin-top: 1px;
    }
    .mode-pop-row .mp-check {
      flex-shrink: 0;
      color: var(--accent);
      font-weight: 700;
      font-size: 14px;
      visibility: hidden;
    }
    .mode-pop-row.selected .mp-check { visibility: visible; }

    /* Effort row at the bottom of the mode popover. Visualizes 4 dots; the
       filled dot reflects the active effort level (Off / Low / Med / High). */
    .mode-pop-effort {
      display: flex; align-items: center; gap: 10px;
      padding: 10px 12px;
      border-top: 1px solid var(--border);
      cursor: pointer;
      color: var(--text);
      font-size: 12.5px;
      transition: background 80ms ease;
    }
    .mode-pop-effort:hover { background: var(--bg-input); }
    .mode-pop-effort .me-label { font-weight: 600; }
    .mode-pop-effort .me-name { color: var(--text-muted); }
    .mode-pop-effort .me-dots {
      margin-left: auto;
      display: inline-flex; align-items: center; gap: 6px;
    }
    /* Radio-dial pattern: every dot is the same muted shape; ONLY the
       active level's dot is filled + slightly larger with a halo. Inactive
       dots are individually clickable to jump straight to that level. */
    .mode-pop-effort .me-dot {
      width: 10px; height: 10px;
      border-radius: 50%;
      background: transparent;
      border: 1.5px solid var(--border);
      cursor: pointer;
      transition: background 80ms ease, border-color 80ms ease,
                  box-shadow 80ms ease, transform 80ms ease;
    }
    .mode-pop-effort .me-dot:hover {
      border-color: var(--text-muted);
      background: var(--bg-input);
    }
    .mode-pop-effort .me-dot.active {
      background: var(--text);
      border-color: var(--text);
      transform: scale(1.15);
      box-shadow: 0 0 0 3px rgba(255,255,255,0.10);
    }

    /* ----- slash command autocomplete popover ----- */
    #slash-popover {
      position: absolute;
      bottom: calc(100% - 1px);
      left: 12px; right: 12px;
      background: var(--bg-elevated);
      border: 1px solid var(--border);
      border-radius: 8px;
      box-shadow: 0 -4px 16px rgba(0,0,0,0.35);
      max-height: 340px;
      overflow: hidden;
      z-index: 50;
      display: none;
      flex-direction: column;
    }
    #slash-popover.open { display: flex; }
    .slash-header {
      padding: 6px 12px;
      background: var(--bg-input);
      border-bottom: 1px solid var(--border-soft);
      font-size: 11px;
      color: var(--text-muted);
      font-family: ui-monospace, Consolas, monospace;
    }
    .slash-list {
      flex: 1 1 auto;
      overflow-y: auto;
      max-height: 180px;
    }
    .slash-item {
      display: flex; align-items: baseline; gap: 12px;
      padding: 9px 14px;
      cursor: pointer;
      border-bottom: 1px solid var(--border-soft);
      transition: background-color 80ms ease, border-color 80ms ease;
    }
    .slash-item:last-child { border-bottom: 0; }
    .slash-item:hover:not(.selected) { background: var(--bg-input); }
    .slash-item.selected {
      background: var(--accent);
      color: var(--accent-fg);
      border-left: 3px solid var(--accent-fg);
      padding-left: 11px;
    }
    .slash-item.selected .slash-desc,
    .slash-item.selected .slash-aliases { color: var(--accent-fg); opacity: 0.9; }
    .slash-name {
      font-weight: 700;
      font-family: ui-monospace, Consolas, monospace;
      flex-shrink: 0;
      min-width: 92px;
    }
    .slash-desc {
      flex: 1;
      color: var(--text-muted);
      font-size: 13px;
    }
    .slash-aliases {
      color: var(--text-dim);
      font-size: 11px;
      font-family: ui-monospace, Consolas, monospace;
    }
    .slash-preview {
      padding: 10px 14px;
      background: var(--bg-input);
      border-top: 1px solid var(--border-soft);
      font-size: 12px;
      color: var(--text);
      line-height: 1.5;
      /* Reserve a stable footprint so hovering between commands with
         different description lengths doesn't bounce the list scroll. */
      min-height: 72px;
      max-height: 110px;
      overflow-y: auto;
    }
    .slash-preview .pv-usage {
      display: inline-block;
      margin-top: 4px;
      font-family: ui-monospace, Consolas, monospace;
      color: var(--text-muted);
      font-size: 11px;
    }
    #slash-hint {
      padding: 6px 12px;
      font-size: 11px;
      color: var(--text-dim);
      border-top: 1px solid var(--border-soft);
      background: var(--bg-input);
    }
    #input {
      width: 100%;
      box-sizing: border-box;
      /* Height is controlled by the splitter bar above the composer
         (see #composer-splitter). resize:none turns off the browser's
         default corner handle. */
      resize: none;
      height: 38px;          /* initial single-line height */
      min-height: 38px;
      padding: 8px 10px;
      border: 1px solid var(--border);
      border-radius: 6px;
      background: var(--bg-input);
      color: var(--text);
      font: inherit;
      outline: none;
    }
    /* Horizontal splitter bar between the messages area and the composer.
       Drag UP to grow the composer; drag DOWN to shrink it back. */
    #composer-splitter {
      flex-shrink: 0;
      height: 5px;
      cursor: ns-resize;
      background: transparent;
      border-top: 1px solid var(--border-soft);
      border-bottom: 1px solid var(--border-soft);
      transition: background 120ms ease-in-out;
      position: relative;
    }
    #composer-splitter:hover,
    #composer-splitter.dragging {
      background: var(--accent);
      border-top-color: var(--accent);
      border-bottom-color: var(--accent);
    }
    /* When the splitter is being dragged, kill text selection and force
       the resize cursor everywhere so the visual doesn't flicker between
       arrow / I-beam mid-drag. */
    body.composer-resizing {
      cursor: ns-resize !important;
      user-select: none;
    }
    #input:focus { border-color: var(--accent); }
    /* Send / stop buttons live in the fixed-height action toolbar. Square
       icon-style buttons; no growing horizontal padding. */
    button.send {
      height: 28px;
      min-width: 28px;
      padding: 0 8px;
      background: var(--accent); color: var(--accent-fg);
      border: 0; border-radius: 6px;
      font-weight: 600; cursor: pointer;
      display: inline-flex; align-items: center; justify-content: center;
    }
    button.send svg {
      width: 14px; height: 14px;
      stroke: currentColor;
      fill: none;
      stroke-width: 2.4;
      stroke-linecap: round;
      stroke-linejoin: round;
    }
    button.send:disabled { opacity: 0.5; cursor: not-allowed; }
    button.stop-btn { background: #c0392b; color: #fff; }
    button.stop-btn:hover { background: #d44638; }
    .save-indicator {
      font-size: 11px;
      color: var(--text-muted);
      height: 14px;
      padding: 0 4px;
      margin: 0 0 4px;
      text-align: right;
      transition: opacity 200ms ease-in-out;
      opacity: 0;
    }
    .save-indicator.show { opacity: 1; }
    .save-indicator.err  { color: #c0392b; }
    .save-indicator.ok::before { content: "✓ "; color: #2d8c4a; }

    /* Thinking indicator. Sits above the composer; flips visible while a
       turn is in flight. The pulsing dot is CSS-only so the "still alive"
       signal keeps animating even if JS is wedged. */
    .thinking-indicator {
      display: none;
      align-items: center;
      gap: 8px;
      font-size: 12px;
      color: var(--text-muted);
      padding: 4px 8px;
      margin: 0 0 4px;
      background: var(--bubble-system-bg, rgba(110, 130, 180, 0.08));
      border-radius: 6px;
    }
    .thinking-indicator.show { display: inline-flex; }
    .thinking-dot {
      width: 8px;
      height: 8px;
      border-radius: 50%;
      background: #4a90e2;
      animation: thinkingPulse 1.2s ease-in-out infinite;
      flex-shrink: 0;
    }
    .thinking-label   { font-weight: 500; }
    .thinking-elapsed {
      margin-left: auto;
      font-variant-numeric: tabular-nums;
      opacity: 0.7;
    }
    @keyframes thinkingPulse {
      0%, 100% { transform: scale(1);   opacity: 0.55; }
      50%      { transform: scale(1.5); opacity: 1; }
    }

    #cost-warning-banner {
      background: rgba(255, 180, 0, 0.12);
      border: 1px solid rgba(255, 180, 0, 0.45);
      color: #d18b00;
      padding: 8px 12px;
      font-size: 12px;
      border-radius: 6px;
      margin: 0 0 8px;
    }

    /* ----- anchor-missing bar — shown when the panel is open without an
       in-model ModelerAI object to anchor the session to. Chat still
       works, but nothing persists across launches. ----- */
    #anchor-bar {
      display: flex;
      align-items: center;
      gap: 10px;
      padding: 6px 14px;
      background: rgba(96, 165, 250, 0.15);
      border-bottom: 1px solid rgba(96, 165, 250, 0.55);
      color: #2563eb;
      font-size: 12.5px;
      flex-shrink: 0;
    }
    #anchor-bar .anchor-icon { font-size: 14px; }
    #anchor-bar #anchor-text { flex: 1; font-weight: 600; }
    #anchor-bar code {
      background: rgba(96, 165, 250, 0.2);
      padding: 1px 5px; border-radius: 3px; font-size: 11px;
    }
    /* ----- Session Memory list (Settings → Session Memory) ----- */
    .sm-list { display: flex; flex-direction: column; gap: 6px; margin: 10px 0; }
    .sm-row {
      display: flex; align-items: center; gap: 10px;
      padding: 8px 10px;
      border: 1px solid var(--border-soft);
      border-radius: 6px;
      background: var(--bg-elevated);
    }
    .sm-row.current { border-color: var(--accent); }
    .sm-row .info { flex: 1; min-width: 0; }
    .sm-row .info .title { font-weight: 600; font-size: 13px; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
    .sm-row .info .meta { color: var(--text-muted); font-size: 11.5px; }
    .sm-row .info .meta code {
      font-size: 10.5px;
      background: var(--surface-3, rgba(127,127,127,0.12));
      padding: 1px 4px; border-radius: 3px;
    }
    .sm-row .badge { flex-shrink: 0; }
    .sm-empty {
      padding: 16px;
      text-align: center;
      color: var(--text-muted);
      border: 1px dashed var(--border-soft);
      border-radius: 6px;
    }
    /* ----- debug drawer (toggle by show_debug_info setting) ----- */
    #debug-drawer {
      border-top: 1px solid var(--border);
      background: var(--bg-sidebar);
      max-height: 280px;
      display: flex; flex-direction: column;
      font: 11px/1.45 ui-monospace, Consolas, "Cascadia Mono", monospace;
    }
    #debug-drawer.collapsed .debug-body { display: none; }
    #debug-drawer.collapsed { max-height: 28px; }
    .debug-head {
      display: flex; align-items: center; gap: 12px;
      padding: 4px 12px;
      background: var(--bg-elevated);
      border-bottom: 1px solid var(--border-soft);
      font-weight: 700; font-size: 11px;
      cursor: pointer;
    }
    .debug-meta { color: var(--text-muted); font-weight: 400; flex: 1; }
    .debug-collapse {
      background: transparent; border: 0; color: var(--text-muted);
      font: 14px monospace; cursor: pointer; padding: 0 4px;
    }
    .debug-body {
      overflow-y: auto;
      padding: 8px 12px;
    }
    .debug-body h4 {
      margin: 4px 0 4px; font-size: 11px;
      color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.5px;
    }
    .debug-body pre {
      background: var(--bg);
      border: 1px solid var(--border-soft);
      border-radius: 4px;
      padding: 6px 8px;
      margin: 0 0 8px;
      white-space: pre-wrap;
      word-break: break-word;
      max-height: 120px;
      overflow-y: auto;
    }

    /* ============================================================
       Settings pane — sidebar layout
       ============================================================ */
    #pane-settings { flex-direction: row; }
    #settings-sidebar {
      width: 160px; flex-shrink: 0;
      background: var(--bg-sidebar);
      border-right: 1px solid var(--border-soft);
      overflow-y: auto;
      padding: 8px 0;
    }
    .sb-item {
      display: block;
      padding: 8px 14px;
      cursor: pointer;
      color: var(--text-muted);
      font-size: 13px; font-weight: 600;
      border-left: 2px solid transparent;
      user-select: none;
    }
    .sb-item:hover { color: var(--text); background: var(--bg-elevated); }
    .sb-item.active {
      color: var(--text);
      border-left-color: var(--accent);
      background: var(--bg-elevated);
    }
    #settings-content {
      flex: 1; overflow-y: auto;
      padding: 20px 24px;
    }
    .sg { display: none; }
    .sg.active { display: block; }
    .sg h2 {
      margin: 0 0 12px 0;
      font-size: 15px; font-weight: 700;
      letter-spacing: 0.01em;
    }
    .sg h3 {
      margin: 18px 0 8px 0;
      font-size: 13px; font-weight: 600;
      color: var(--text);
    }
    .sg .meta {
      color: var(--text-muted); font-size: 13px; line-height: 1.6;
      margin-bottom: 12px;
    }
    .sg code {
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      border-radius: 3px;
      padding: 1px 5px;
      font: 12px ui-monospace, Consolas, monospace;
    }
    .sg .row {
      display: flex; align-items: center; gap: 10px;
      padding: 8px 0;
    }
    .sg .row + .row { border-top: 1px solid var(--border-soft); }
    .sg .row .label { flex: 1; }
    .sg .row .label .name { font-weight: 600; }
    .sg .row .label .desc { color: var(--text-muted); font-size: 12px; }
    .sg button {
      padding: 6px 12px;
      background: var(--bg-input);
      color: var(--text);
      border: 1px solid var(--border);
      border-radius: 5px;
      cursor: pointer;
      font: inherit; font-size: 13px;
    }
    .sg button:hover { border-color: var(--accent); color: var(--accent); }
    .sg button.primary {
      background: var(--accent); color: var(--accent-fg);
      border-color: var(--accent);
    }
    .sg button.primary:hover { background: var(--accent-hover); color: var(--accent-fg); }
    .sg button.danger { color: var(--error); border-color: var(--error); }
    .sg button.danger:hover { background: var(--error); color: #fff; border-color: var(--error); }
    .sg select {
      padding: 6px 8px;
      background: var(--bg-input);
      color: var(--text);
      border: 1px solid var(--border);
      border-radius: 5px;
      font: inherit; font-size: 13px;
    }
    .sg input[type="text"] {
      padding: 6px 8px;
      background: var(--bg-input);
      color: var(--text);
      border: 1px solid var(--border);
      border-radius: 5px;
      font: inherit; font-size: 13px;
      min-width: 220px;
    }
    .badge {
      display: inline-block;
      padding: 2px 8px;
      border-radius: 999px;
      font-size: 11px; font-weight: 600;
      border: 1px solid transparent;
    }
    .badge-ok        { background: rgba(74,222,128,0.15);  color: var(--success); border-color: var(--success); }
    .badge-warn      { background: rgba(96,165,250,0.15);  color: var(--accent);  border-color: var(--accent); }
    .badge-bad       { background: rgba(248,113,113,0.15); color: var(--error);   border-color: var(--error); }
    .badge-grey      { background: var(--bg-input);        color: var(--text-muted); border-color: var(--border); }
    .stat-grid {
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 10px;
      margin: 12px 0;
    }
    .stat {
      background: var(--bg-elevated);
      border: 1px solid var(--border-soft);
      border-radius: 6px;
      padding: 10px 12px;
    }
    .stat .val { font-size: 18px; font-weight: 700; }
    .stat .lbl { font-size: 11px; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.05em; }
    .pill-list { display: flex; flex-wrap: wrap; gap: 6px; margin-top: 6px; }
    .pill-list .badge { font-size: 10px; }
    .placeholder-block {
      padding: 14px 16px;
      background: var(--bg-input);
      border: 1px dashed var(--border);
      border-radius: 6px;
      color: var(--text-muted);
      font-size: 13px;
    }
    .preview-box {
      background: var(--bg-input);
      border: 1px solid var(--border-soft);
      border-radius: 5px;
      padding: 10px;
      font: 12px ui-monospace, Consolas, monospace;
      white-space: pre-wrap;
      max-height: 180px; overflow-y: auto;
      color: var(--text-muted);
    }
    .disabled-row { opacity: 0.5; }
    .disabled-row .badge-grey { opacity: 1; }
    .radio-group label {
      display: inline-flex; align-items: center; gap: 6px;
      margin-right: 16px;
      cursor: pointer;
    }
    /* Vertical-stacked radios (used in Sessions). Each row gets its own
       padding so descriptions wrap cleanly without a &lt;br&gt;. */
    .radio-group.stacked label {
      display: flex; align-items: flex-start; gap: 8px;
      margin: 0; padding: 6px 0;
      flex-wrap: wrap;
    }
    .radio-group.stacked label &gt; input { margin-top: 4px; }
    .meta-inline { color: var(--text-muted); font-size: 12px; }
    .radio-group label &gt; strong { color: var(--text); }
    .form-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 8px 12px;
      margin: 8px 0 12px;
    }
    .form-grid label {
      display: flex; flex-direction: column; gap: 4px;
      font-size: 12px; color: var(--text-muted);
    }
    .form-grid input {
      padding: 6px 8px;
      background: var(--bg-input);
      color: var(--text);
      border: 1px solid var(--border);
      border-radius: 5px;
      font: inherit; font-size: 13px;
    }
    .form-grid input:focus { border-color: var(--accent); outline: none; }
    .provider-row {
      display: flex; align-items: center; gap: 10px;
      padding: 8px 0;
      border-top: 1px solid var(--border-soft);
    }
    .provider-row:first-child { border-top: 0; }
    .provider-row .info { flex: 1; }
    .provider-row .info .name { font-weight: 600; }
    .provider-row .info .desc { color: var(--text-muted); font-size: 12px; }
    .provider-row .info .desc code { font-size: 11px; }
    /* Setup sub-panel: grid of provider-overview cards. */
    .setup-providers {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
      gap: 14px;
      margin: 14px 0 20px 0;
    }
    .setup-card {
      border: 1px solid var(--border-soft);
      border-radius: 8px;
      padding: 12px 14px;
      background: var(--surface-2, transparent);
    }
    .setup-card h4 {
      margin: 0 0 6px 0;
      font-size: 14px;
    }
    .setup-card .meta { margin-bottom: 10px; }
    .setup-card .meta a { color: var(--accent); text-decoration: none; }
    .setup-card .meta a:hover { text-decoration: underline; }
    .setup-key-options {
      font-size: 12px;
      display: flex; flex-direction: column; gap: 4px;
      padding: 8px;
      border-top: 1px dashed var(--border-soft);
    }
    .setup-key-options code {
      background: var(--surface-3, rgba(127,127,127,0.12));
      padding: 1px 5px; border-radius: 3px; font-size: 11px;
    }

    #remote-disconnect-overlay {
      display: none;
      position: fixed; inset: 0; z-index: 99999;
      background: rgba(0, 0, 0, 0.65);
      align-items: center; justify-content: center;
      flex-direction: column;
    }
    #remote-disconnect-overlay.show { display: flex; }
    #remote-disconnect-card {
      background: var(--surface-bg, #2a2a2e);
      border-radius: 12px; padding: 32px;
      max-width: 360px; text-align: center;
      color: var(--text-fg, #fff);
    }
    #remote-disconnect-card h3 { margin: 0 0 12px; }
    #remote-disconnect-card p  { margin: 0 0 18px; opacity: 0.85; }
    #remote-disconnect-card button {
      padding: 10px 18px; border-radius: 8px;
      background: #4a90e2; color: white; border: none; font-size: 14px;
      cursor: pointer;
    }

    /* ============================================================
       Mobile touch sizing — at least 44 px on the interactive bits
       (tabs, composer toolbar, send/stop, model &amp; mode pills,
       sidebar items, and the chat-header model pill).
       ============================================================ */
    @media (max-width: 640px) {
      .tab,
      .sb-item,
      .composer-toolbar-btn,
      .composer-action-btn,
      .model-pill,
      .send,
      .stop-btn {
        min-height: 44px;
        min-width:  44px;
      }
      /* Tabs are normally compact; give them real padding on touch. */
      .tab { padding: 12px 18px; }
      /* Sidebar items: wider tap area when collapsed to a row. */
      .sb-item { padding: 12px 14px; }
    }

    /* ============================================================
       Mobile single-column layout — settings sidebar collapses to
       a horizontal scroll at the top of the settings pane; composer
       toolbar drops the text labels in favor of icons.
       ============================================================ */
    @media (max-width: 640px) {
      /* Settings pane: stack sidebar above content instead of side-by-side. */
      #pane-settings { flex-direction: column; }
      #settings-sidebar {
        width: 100%;
        max-width: none;
        display: flex; flex-direction: row;
        overflow-x: auto; overflow-y: hidden;
        border-right: 0;
        border-bottom: 1px solid var(--border-soft);
        padding: 4px 4px;
        flex-shrink: 0;
        white-space: nowrap;
      }
      #settings-sidebar .sb-item {
        flex: 0 0 auto;
        border-left: 0;
        border-bottom: 2px solid transparent;
      }
      #settings-sidebar .sb-item.active {
        border-left-color: transparent;
        border-bottom-color: var(--accent);
      }
      #settings-content { padding: 14px 14px; }

      /* Composer toolbar: hide text labels, keep icons + carets. */
      .composer-toolbar-btn .tb-label { display: none; }

      /* Messages: tighten padding so bubbles get more room. */
      #messages { padding-left: 8px; padding-right: 8px; }
    }
    @media (max-width: 480px) {
      /* Further compression on tiny screens — drop the caret too,
         leaving an icon-only model/mode button. */
      .composer-toolbar-btn .tb-caret { display: none; }
      .tab { padding: 12px 12px; font-size: 12px; }
    }

    /* Hamburger toggle — hidden on desktop, shown in the tab bar on
       mobile. Clicking it jumps to the Settings pane. */
    .header-mobile-only {
      display: none;
      background: transparent;
      color: var(--text);
      border: 0;
      font-size: 20px; line-height: 1;
      cursor: pointer;
      user-select: none;
    }
    @media (max-width: 640px) {
      .header-mobile-only {
        display: inline-flex;
        align-items: center; justify-content: center;
        min-height: 44px; min-width: 44px;
        padding: 0 12px;
      }
    }

    /* ============================================================
       Phone-mode UI gate (?mode=phone)
       Stripped view served to remote phone clients via Cloudflare
       Tunnel. Same JS app — body.phone-mode hides host-only controls.
       C++ side rejects host-only envelopes from non-local subscribers
       separately (see bridge::handleEnvelope, Task 1.10).

       Always visible on phone: transcript (#messages), compose textarea
       (#input), Send + Stop (#composer-actions-right), reconnect overlay
       (#remote-disconnect-overlay).
       ============================================================ */

    /* Hide the entire tabbar (Chat/Settings tabs + hamburger + remote
       badge). Phone clients are always in chat view; Settings is not
       accessible on phone, so the tab strip serves no purpose. */
    body.phone-mode #tabbar {
      display: none !important;
    }

    /* Hide the settings pane. Hides provider picker, model picker (in
       Settings → Model), mode selector (Settings → General), cost meter
       (Settings → Usage), session export/clear (Settings → Sessions),
       and remote-panel sharing controls (Settings → Remote) all at once. */
    body.phone-mode #pane-settings {
      display: none !important;
    }

    /* Composer-actions-left (attach / slash / model / mode picker buttons)
       stays VISIBLE on phones — Josh wants the same chat affordances as
       the host UI. Only Settings (full pane) is host-only. */

    /* Restructure the composer for phone mode so Send (the enter button)
       sits to the SIDE of the textarea instead of below it. Default
       host layout is column-stacked (textarea row 1, actions row 2).
       Phone layout via grid:
            [textarea ........................] [Send/Stop]
            [+  /  Model  Mode]
       display:contents on #composer-actions lets its children participate
       in the parent grid without touching the HTML. */
    body.phone-mode #composer {
      display: grid;
      grid-template-columns: 1fr auto;
      grid-template-rows: auto auto auto;
      grid-template-areas:
        "atts  atts"
        "input send"
        "left  left";
      gap: 6px;
      padding: 8px 10px 10px;
      align-items: end;
    }
    body.phone-mode #composer #attachments-row     { grid-area: atts; }
    body.phone-mode #composer #attach-input        { grid-area: atts; }
    body.phone-mode #composer textarea#input       { grid-area: input; }
    body.phone-mode #composer #composer-actions    { display: contents; }
    body.phone-mode #composer #composer-actions-left  { grid-area: left;
                                                         height: 34px; }
    body.phone-mode #composer #composer-actions-right { grid-area: send;
                                                         align-self: end;
                                                         height: 34px; }

    /* Tighten layout on small screens. 16px is the iOS Safari magic
       value that prevents auto-zoom on input focus. */
    body.phone-mode #input,
    body.phone-mode textarea,
    body.phone-mode input[type="text"] {
      font-size: 16px;
    }

    body.phone-mode #composer-wrap {
      padding-top: 0;
    }

  &lt;/style&gt;
  &lt;script&gt;
    // qrcode-generator vendored (v1.4.4) — see webview/vendor/qrcode-generator.js
    // for source-of-truth. Inlined here because tools/embed.js only handles
    // index.html, so the local CEF viewer cannot fetch /vendor/* assets.
//---------------------------------------------------------------------
//
// QR Code Generator for JavaScript
//
// Copyright (c) 2009 Kazuhiko Arase
//
// URL: http://www.d-project.com/
//
// Licensed under the MIT license:
//  http://www.opensource.org/licenses/mit-license.php
//
// The word 'QR Code' is registered trademark of
// DENSO WAVE INCORPORATED
//  http://www.denso-wave.com/qrcode/faqpatent-e.html
//
//---------------------------------------------------------------------

var qrcode = function() {

  //---------------------------------------------------------------------
  // qrcode
  //---------------------------------------------------------------------

  /**
   * qrcode
   * @param typeNumber 1 to 40
   * @param errorCorrectionLevel 'L','M','Q','H'
   */
  var qrcode = function(typeNumber, errorCorrectionLevel) {

    var PAD0 = 0xEC;
    var PAD1 = 0x11;

    var _typeNumber = typeNumber;
    var _errorCorrectionLevel = QRErrorCorrectionLevel[errorCorrectionLevel];
    var _modules = null;
    var _moduleCount = 0;
    var _dataCache = null;
    var _dataList = [];

    var _this = {};

    var makeImpl = function(test, maskPattern) {

      _moduleCount = _typeNumber * 4 + 17;
      _modules = function(moduleCount) {
        var modules = new Array(moduleCount);
        for (var row = 0; row &lt; moduleCount; row += 1) {
          modules[row] = new Array(moduleCount);
          for (var col = 0; col &lt; moduleCount; col += 1) {
            modules[row][col] = null;
          }
        }
        return modules;
      }(_moduleCount);

      setupPositionProbePattern(0, 0);
      setupPositionProbePattern(_moduleCount - 7, 0);
      setupPositionProbePattern(0, _moduleCount - 7);
      setupPositionAdjustPattern();
      setupTimingPattern();
      setupTypeInfo(test, maskPattern);

      if (_typeNumber &gt;= 7) {
        setupTypeNumber(test);
      }

      if (_dataCache == null) {
        _dataCache = createData(_typeNumber, _errorCorrectionLevel, _dataList);
      }

      mapData(_dataCache, maskPattern);
    };

    var setupPositionProbePattern = function(row, col) {

      for (var r = -1; r &lt;= 7; r += 1) {

        if (row + r &lt;= -1 || _moduleCount &lt;= row + r) continue;

        for (var c = -1; c &lt;= 7; c += 1) {

          if (col + c &lt;= -1 || _moduleCount &lt;= col + c) continue;

          if ( (0 &lt;= r &amp;&amp; r &lt;= 6 &amp;&amp; (c == 0 || c == 6) )
              || (0 &lt;= c &amp;&amp; c &lt;= 6 &amp;&amp; (r == 0 || r == 6) )
              || (2 &lt;= r &amp;&amp; r &lt;= 4 &amp;&amp; 2 &lt;= c &amp;&amp; c &lt;= 4) ) {
            _modules[row + r][col + c] = true;
          } else {
            _modules[row + r][col + c] = false;
          }
        }
      }
    };

    var getBestMaskPattern = function() {

      var minLostPoint = 0;
      var pattern = 0;

      for (var i = 0; i &lt; 8; i += 1) {

        makeImpl(true, i);

        var lostPoint = QRUtil.getLostPoint(_this);

        if (i == 0 || minLostPoint &gt; lostPoint) {
          minLostPoint = lostPoint;
          pattern = i;
        }
      }

      return pattern;
    };

    var setupTimingPattern = function() {

      for (var r = 8; r &lt; _moduleCount - 8; r += 1) {
        if (_modules[r][6] != null) {
          continue;
        }
        _modules[r][6] = (r % 2 == 0);
      }

      for (var c = 8; c &lt; _moduleCount - 8; c += 1) {
        if (_modules[6][c] != null) {
          continue;
        }
        _modules[6][c] = (c % 2 == 0);
      }
    };

    var setupPositionAdjustPattern = function() {

      var pos = QRUtil.getPatternPosition(_typeNumber);

      for (var i = 0; i &lt; pos.length; i += 1) {

        for (var j = 0; j &lt; pos.length; j += 1) {

          var row = pos[i];
          var col = pos[j];

          if (_modules[row][col] != null) {
            continue;
          }

          for (var r = -2; r &lt;= 2; r += 1) {

            for (var c = -2; c &lt;= 2; c += 1) {

              if (r == -2 || r == 2 || c == -2 || c == 2
                  || (r == 0 &amp;&amp; c == 0) ) {
                _modules[row + r][col + c] = true;
              } else {
                _modules[row + r][col + c] = false;
              }
            }
          }
        }
      }
    };

    var setupTypeNumber = function(test) {

      var bits = QRUtil.getBCHTypeNumber(_typeNumber);

      for (var i = 0; i &lt; 18; i += 1) {
        var mod = (!test &amp;&amp; ( (bits &gt;&gt; i) &amp; 1) == 1);
        _modules[Math.floor(i / 3)][i % 3 + _moduleCount - 8 - 3] = mod;
      }

      for (var i = 0; i &lt; 18; i += 1) {
        var mod = (!test &amp;&amp; ( (bits &gt;&gt; i) &amp; 1) == 1);
        _modules[i % 3 + _moduleCount - 8 - 3][Math.floor(i / 3)] = mod;
      }
    };

    var setupTypeInfo = function(test, maskPattern) {

      var data = (_errorCorrectionLevel &lt;&lt; 3) | maskPattern;
      var bits = QRUtil.getBCHTypeInfo(data);

      // vertical
      for (var i = 0; i &lt; 15; i += 1) {

        var mod = (!test &amp;&amp; ( (bits &gt;&gt; i) &amp; 1) == 1);

        if (i &lt; 6) {
          _modules[i][8] = mod;
        } else if (i &lt; 8) {
          _modules[i + 1][8] = mod;
        } else {
          _modules[_moduleCount - 15 + i][8] = mod;
        }
      }

      // horizontal
      for (var i = 0; i &lt; 15; i += 1) {

        var mod = (!test &amp;&amp; ( (bits &gt;&gt; i) &amp; 1) == 1);

        if (i &lt; 8) {
          _modules[8][_moduleCount - i - 1] = mod;
        } else if (i &lt; 9) {
          _modules[8][15 - i - 1 + 1] = mod;
        } else {
          _modules[8][15 - i - 1] = mod;
        }
      }

      // fixed module
      _modules[_moduleCount - 8][8] = (!test);
    };

    var mapData = function(data, maskPattern) {

      var inc = -1;
      var row = _moduleCount - 1;
      var bitIndex = 7;
      var byteIndex = 0;
      var maskFunc = QRUtil.getMaskFunction(maskPattern);

      for (var col = _moduleCount - 1; col &gt; 0; col -= 2) {

        if (col == 6) col -= 1;

        while (true) {

          for (var c = 0; c &lt; 2; c += 1) {

            if (_modules[row][col - c] == null) {

              var dark = false;

              if (byteIndex &lt; data.length) {
                dark = ( ( (data[byteIndex] &gt;&gt;&gt; bitIndex) &amp; 1) == 1);
              }

              var mask = maskFunc(row, col - c);

              if (mask) {
                dark = !dark;
              }

              _modules[row][col - c] = dark;
              bitIndex -= 1;

              if (bitIndex == -1) {
                byteIndex += 1;
                bitIndex = 7;
              }
            }
          }

          row += inc;

          if (row &lt; 0 || _moduleCount &lt;= row) {
            row -= inc;
            inc = -inc;
            break;
          }
        }
      }
    };

    var createBytes = function(buffer, rsBlocks) {

      var offset = 0;

      var maxDcCount = 0;
      var maxEcCount = 0;

      var dcdata = new Array(rsBlocks.length);
      var ecdata = new Array(rsBlocks.length);

      for (var r = 0; r &lt; rsBlocks.length; r += 1) {

        var dcCount = rsBlocks[r].dataCount;
        var ecCount = rsBlocks[r].totalCount - dcCount;

        maxDcCount = Math.max(maxDcCount, dcCount);
        maxEcCount = Math.max(maxEcCount, ecCount);

        dcdata[r] = new Array(dcCount);

        for (var i = 0; i &lt; dcdata[r].length; i += 1) {
          dcdata[r][i] = 0xff &amp; buffer.getBuffer()[i + offset];
        }
        offset += dcCount;

        var rsPoly = QRUtil.getErrorCorrectPolynomial(ecCount);
        var rawPoly = qrPolynomial(dcdata[r], rsPoly.getLength() - 1);

        var modPoly = rawPoly.mod(rsPoly);
        ecdata[r] = new Array(rsPoly.getLength() - 1);
        for (var i = 0; i &lt; ecdata[r].length; i += 1) {
          var modIndex = i + modPoly.getLength() - ecdata[r].length;
          ecdata[r][i] = (modIndex &gt;= 0)? modPoly.getAt(modIndex) : 0;
        }
      }

      var totalCodeCount = 0;
      for (var i = 0; i &lt; rsBlocks.length; i += 1) {
        totalCodeCount += rsBlocks[i].totalCount;
      }

      var data = new Array(totalCodeCount);
      var index = 0;

      for (var i = 0; i &lt; maxDcCount; i += 1) {
        for (var r = 0; r &lt; rsBlocks.length; r += 1) {
          if (i &lt; dcdata[r].length) {
            data[index] = dcdata[r][i];
            index += 1;
          }
        }
      }

      for (var i = 0; i &lt; maxEcCount; i += 1) {
        for (var r = 0; r &lt; rsBlocks.length; r += 1) {
          if (i &lt; ecdata[r].length) {
            data[index] = ecdata[r][i];
            index += 1;
          }
        }
      }

      return data;
    };

    var createData = function(typeNumber, errorCorrectionLevel, dataList) {

      var rsBlocks = QRRSBlock.getRSBlocks(typeNumber, errorCorrectionLevel);

      var buffer = qrBitBuffer();

      for (var i = 0; i &lt; dataList.length; i += 1) {
        var data = dataList[i];
        buffer.put(data.getMode(), 4);
        buffer.put(data.getLength(), QRUtil.getLengthInBits(data.getMode(), typeNumber) );
        data.write(buffer);
      }

      // calc num max data.
      var totalDataCount = 0;
      for (var i = 0; i &lt; rsBlocks.length; i += 1) {
        totalDataCount += rsBlocks[i].dataCount;
      }

      if (buffer.getLengthInBits() &gt; totalDataCount * 8) {
        throw 'code length overflow. ('
          + buffer.getLengthInBits()
          + '&gt;'
          + totalDataCount * 8
          + ')';
      }

      // end code
      if (buffer.getLengthInBits() + 4 &lt;= totalDataCount * 8) {
        buffer.put(0, 4);
      }

      // padding
      while (buffer.getLengthInBits() % 8 != 0) {
        buffer.putBit(false);
      }

      // padding
      while (true) {

        if (buffer.getLengthInBits() &gt;= totalDataCount * 8) {
          break;
        }
        buffer.put(PAD0, 8);

        if (buffer.getLengthInBits() &gt;= totalDataCount * 8) {
          break;
        }
        buffer.put(PAD1, 8);
      }

      return createBytes(buffer, rsBlocks);
    };

    _this.addData = function(data, mode) {

      mode = mode || 'Byte';

      var newData = null;

      switch(mode) {
      case 'Numeric' :
        newData = qrNumber(data);
        break;
      case 'Alphanumeric' :
        newData = qrAlphaNum(data);
        break;
      case 'Byte' :
        newData = qr8BitByte(data);
        break;
      case 'Kanji' :
        newData = qrKanji(data);
        break;
      default :
        throw 'mode:' + mode;
      }

      _dataList.push(newData);
      _dataCache = null;
    };

    _this.isDark = function(row, col) {
      if (row &lt; 0 || _moduleCount &lt;= row || col &lt; 0 || _moduleCount &lt;= col) {
        throw row + ',' + col;
      }
      return _modules[row][col];
    };

    _this.getModuleCount = function() {
      return _moduleCount;
    };

    _this.make = function() {
      if (_typeNumber &lt; 1) {
        var typeNumber = 1;

        for (; typeNumber &lt; 40; typeNumber++) {
          var rsBlocks = QRRSBlock.getRSBlocks(typeNumber, _errorCorrectionLevel);
          var buffer = qrBitBuffer();

          for (var i = 0; i &lt; _dataList.length; i++) {
            var data = _dataList[i];
            buffer.put(data.getMode(), 4);
            buffer.put(data.getLength(), QRUtil.getLengthInBits(data.getMode(), typeNumber) );
            data.write(buffer);
          }

          var totalDataCount = 0;
          for (var i = 0; i &lt; rsBlocks.length; i++) {
            totalDataCount += rsBlocks[i].dataCount;
          }

          if (buffer.getLengthInBits() &lt;= totalDataCount * 8) {
            break;
          }
        }

        _typeNumber = typeNumber;
      }

      makeImpl(false, getBestMaskPattern() );
    };

    _this.createTableTag = function(cellSize, margin) {

      cellSize = cellSize || 2;
      margin = (typeof margin == 'undefined')? cellSize * 4 : margin;

      var qrHtml = '';

      qrHtml += '&lt;table style="';
      qrHtml += ' border-width: 0px; border-style: none;';
      qrHtml += ' border-collapse: collapse;';
      qrHtml += ' padding: 0px; margin: ' + margin + 'px;';
      qrHtml += '"&gt;';
      qrHtml += '&lt;tbody&gt;';

      for (var r = 0; r &lt; _this.getModuleCount(); r += 1) {

        qrHtml += '&lt;tr&gt;';

        for (var c = 0; c &lt; _this.getModuleCount(); c += 1) {
          qrHtml += '&lt;td style="';
          qrHtml += ' border-width: 0px; border-style: none;';
          qrHtml += ' border-collapse: collapse;';
          qrHtml += ' padding: 0px; margin: 0px;';
          qrHtml += ' width: ' + cellSize + 'px;';
          qrHtml += ' height: ' + cellSize + 'px;';
          qrHtml += ' background-color: ';
          qrHtml += _this.isDark(r, c)? '#000000' : '#ffffff';
          qrHtml += ';';
          qrHtml += '"/&gt;';
        }

        qrHtml += '&lt;/tr&gt;';
      }

      qrHtml += '&lt;/tbody&gt;';
      qrHtml += '&lt;/table&gt;';

      return qrHtml;
    };

    _this.createSvgTag = function(cellSize, margin, alt, title) {

      var opts = {};
      if (typeof arguments[0] == 'object') {
        // Called by options.
        opts = arguments[0];
        // overwrite cellSize and margin.
        cellSize = opts.cellSize;
        margin = opts.margin;
        alt = opts.alt;
        title = opts.title;
      }

      cellSize = cellSize || 2;
      margin = (typeof margin == 'undefined')? cellSize * 4 : margin;

      // Compose alt property surrogate
      alt = (typeof alt === 'string') ? {text: alt} : alt || {};
      alt.text = alt.text || null;
      alt.id = (alt.text) ? alt.id || 'qrcode-description' : null;

      // Compose title property surrogate
      title = (typeof title === 'string') ? {text: title} : title || {};
      title.text = title.text || null;
      title.id = (title.text) ? title.id || 'qrcode-title' : null;

      var size = _this.getModuleCount() * cellSize + margin * 2;
      var c, mc, r, mr, qrSvg='', rect;

      rect = 'l' + cellSize + ',0 0,' + cellSize +
        ' -' + cellSize + ',0 0,-' + cellSize + 'z ';

      qrSvg += '&lt;svg version="1.1" xmlns="http://www.w3.org/2000/svg"';
      qrSvg += !opts.scalable ? ' width="' + size + 'px" height="' + size + 'px"' : '';
      qrSvg += ' viewBox="0 0 ' + size + ' ' + size + '" ';
      qrSvg += ' preserveAspectRatio="xMinYMin meet"';
      qrSvg += (title.text || alt.text) ? ' role="img" aria-labelledby="' +
          escapeXml([title.id, alt.id].join(' ').trim() ) + '"' : '';
      qrSvg += '&gt;';
      qrSvg += (title.text) ? '&lt;title id="' + escapeXml(title.id) + '"&gt;' +
          escapeXml(title.text) + '&lt;/title&gt;' : '';
      qrSvg += (alt.text) ? '&lt;description id="' + escapeXml(alt.id) + '"&gt;' +
          escapeXml(alt.text) + '&lt;/description&gt;' : '';
      qrSvg += '&lt;rect width="100%" height="100%" fill="white" cx="0" cy="0"/&gt;';
      qrSvg += '&lt;path d="';

      for (r = 0; r &lt; _this.getModuleCount(); r += 1) {
        mr = r * cellSize + margin;
        for (c = 0; c &lt; _this.getModuleCount(); c += 1) {
          if (_this.isDark(r, c) ) {
            mc = c*cellSize+margin;
            qrSvg += 'M' + mc + ',' + mr + rect;
          }
        }
      }

      qrSvg += '" stroke="transparent" fill="black"/&gt;';
      qrSvg += '&lt;/svg&gt;';

      return qrSvg;
    };

    _this.createDataURL = function(cellSize, margin) {

      cellSize = cellSize || 2;
      margin = (typeof margin == 'undefined')? cellSize * 4 : margin;

      var size = _this.getModuleCount() * cellSize + margin * 2;
      var min = margin;
      var max = size - margin;

      return createDataURL(size, size, function(x, y) {
        if (min &lt;= x &amp;&amp; x &lt; max &amp;&amp; min &lt;= y &amp;&amp; y &lt; max) {
          var c = Math.floor( (x - min) / cellSize);
          var r = Math.floor( (y - min) / cellSize);
          return _this.isDark(r, c)? 0 : 1;
        } else {
          return 1;
        }
      } );
    };

    _this.createImgTag = function(cellSize, margin, alt) {

      cellSize = cellSize || 2;
      margin = (typeof margin == 'undefined')? cellSize * 4 : margin;

      var size = _this.getModuleCount() * cellSize + margin * 2;

      var img = '';
      img += '&lt;img';
      img += '\u0020src="';
      img += _this.createDataURL(cellSize, margin);
      img += '"';
      img += '\u0020width="';
      img += size;
      img += '"';
      img += '\u0020height="';
      img += size;
      img += '"';
      if (alt) {
        img += '\u0020alt="';
        img += escapeXml(alt);
        img += '"';
      }
      img += '/&gt;';

      return img;
    };

    var escapeXml = function(s) {
      var escaped = '';
      for (var i = 0; i &lt; s.length; i += 1) {
        var c = s.charAt(i);
        switch(c) {
        case '&lt;': escaped += '&amp;lt;'; break;
        case '&gt;': escaped += '&amp;gt;'; break;
        case '&amp;': escaped += '&amp;amp;'; break;
        case '"': escaped += '&amp;quot;'; break;
        default : escaped += c; break;
        }
      }
      return escaped;
    };

    var _createHalfASCII = function(margin) {
      var cellSize = 1;
      margin = (typeof margin == 'undefined')? cellSize * 2 : margin;

      var size = _this.getModuleCount() * cellSize + margin * 2;
      var min = margin;
      var max = size - margin;

      var y, x, r1, r2, p;

      var blocks = {
        '██': '█',
        '█ ': '▀',
        ' █': '▄',
        '  ': ' '
      };

      var blocksLastLineNoMargin = {
        '██': '▀',
        '█ ': '▀',
        ' █': ' ',
        '  ': ' '
      };

      var ascii = '';
      for (y = 0; y &lt; size; y += 2) {
        r1 = Math.floor((y - min) / cellSize);
        r2 = Math.floor((y + 1 - min) / cellSize);
        for (x = 0; x &lt; size; x += 1) {
          p = '█';

          if (min &lt;= x &amp;&amp; x &lt; max &amp;&amp; min &lt;= y &amp;&amp; y &lt; max &amp;&amp; _this.isDark(r1, Math.floor((x - min) / cellSize))) {
            p = ' ';
          }

          if (min &lt;= x &amp;&amp; x &lt; max &amp;&amp; min &lt;= y+1 &amp;&amp; y+1 &lt; max &amp;&amp; _this.isDark(r2, Math.floor((x - min) / cellSize))) {
            p += ' ';
          }
          else {
            p += '█';
          }

          // Output 2 characters per pixel, to create full square. 1 character per pixels gives only half width of square.
          ascii += (margin &lt; 1 &amp;&amp; y+1 &gt;= max) ? blocksLastLineNoMargin[p] : blocks[p];
        }

        ascii += '\n';
      }

      if (size % 2 &amp;&amp; margin &gt; 0) {
        return ascii.substring(0, ascii.length - size - 1) + Array(size+1).join('▀');
      }

      return ascii.substring(0, ascii.length-1);
    };

    _this.createASCII = function(cellSize, margin) {
      cellSize = cellSize || 1;

      if (cellSize &lt; 2) {
        return _createHalfASCII(margin);
      }

      cellSize -= 1;
      margin = (typeof margin == 'undefined')? cellSize * 2 : margin;

      var size = _this.getModuleCount() * cellSize + margin * 2;
      var min = margin;
      var max = size - margin;

      var y, x, r, p;

      var white = Array(cellSize+1).join('██');
      var black = Array(cellSize+1).join('  ');

      var ascii = '';
      var line = '';
      for (y = 0; y &lt; size; y += 1) {
        r = Math.floor( (y - min) / cellSize);
        line = '';
        for (x = 0; x &lt; size; x += 1) {
          p = 1;

          if (min &lt;= x &amp;&amp; x &lt; max &amp;&amp; min &lt;= y &amp;&amp; y &lt; max &amp;&amp; _this.isDark(r, Math.floor((x - min) / cellSize))) {
            p = 0;
          }

          // Output 2 characters per pixel, to create full square. 1 character per pixels gives only half width of square.
          line += p ? white : black;
        }

        for (r = 0; r &lt; cellSize; r += 1) {
          ascii += line + '\n';
        }
      }

      return ascii.substring(0, ascii.length-1);
    };

    _this.renderTo2dContext = function(context, cellSize) {
      cellSize = cellSize || 2;
      var length = _this.getModuleCount();
      for (var row = 0; row &lt; length; row++) {
        for (var col = 0; col &lt; length; col++) {
          context.fillStyle = _this.isDark(row, col) ? 'black' : 'white';
          context.fillRect(row * cellSize, col * cellSize, cellSize, cellSize);
        }
      }
    }

    return _this;
  };

  //---------------------------------------------------------------------
  // qrcode.stringToBytes
  //---------------------------------------------------------------------

  qrcode.stringToBytesFuncs = {
    'default' : function(s) {
      var bytes = [];
      for (var i = 0; i &lt; s.length; i += 1) {
        var c = s.charCodeAt(i);
        bytes.push(c &amp; 0xff);
      }
      return bytes;
    }
  };

  qrcode.stringToBytes = qrcode.stringToBytesFuncs['default'];

  //---------------------------------------------------------------------
  // qrcode.createStringToBytes
  //---------------------------------------------------------------------

  /**
   * @param unicodeData base64 string of byte array.
   * [16bit Unicode],[16bit Bytes], ...
   * @param numChars
   */
  qrcode.createStringToBytes = function(unicodeData, numChars) {

    // create conversion map.

    var unicodeMap = function() {

      var bin = base64DecodeInputStream(unicodeData);
      var read = function() {
        var b = bin.read();
        if (b == -1) throw 'eof';
        return b;
      };

      var count = 0;
      var unicodeMap = {};
      while (true) {
        var b0 = bin.read();
        if (b0 == -1) break;
        var b1 = read();
        var b2 = read();
        var b3 = read();
        var k = String.fromCharCode( (b0 &lt;&lt; 8) | b1);
        var v = (b2 &lt;&lt; 8) | b3;
        unicodeMap[k] = v;
        count += 1;
      }
      if (count != numChars) {
        throw count + ' != ' + numChars;
      }

      return unicodeMap;
    }();

    var unknownChar = '?'.charCodeAt(0);

    return function(s) {
      var bytes = [];
      for (var i = 0; i &lt; s.length; i += 1) {
        var c = s.charCodeAt(i);
        if (c &lt; 128) {
          bytes.push(c);
        } else {
          var b = unicodeMap[s.charAt(i)];
          if (typeof b == 'number') {
            if ( (b &amp; 0xff) == b) {
              // 1byte
              bytes.push(b);
            } else {
              // 2bytes
              bytes.push(b &gt;&gt;&gt; 8);
              bytes.push(b &amp; 0xff);
            }
          } else {
            bytes.push(unknownChar);
          }
        }
      }
      return bytes;
    };
  };

  //---------------------------------------------------------------------
  // QRMode
  //---------------------------------------------------------------------

  var QRMode = {
    MODE_NUMBER :    1 &lt;&lt; 0,
    MODE_ALPHA_NUM : 1 &lt;&lt; 1,
    MODE_8BIT_BYTE : 1 &lt;&lt; 2,
    MODE_KANJI :     1 &lt;&lt; 3
  };

  //---------------------------------------------------------------------
  // QRErrorCorrectionLevel
  //---------------------------------------------------------------------

  var QRErrorCorrectionLevel = {
    L : 1,
    M : 0,
    Q : 3,
    H : 2
  };

  //---------------------------------------------------------------------
  // QRMaskPattern
  //---------------------------------------------------------------------

  var QRMaskPattern = {
    PATTERN000 : 0,
    PATTERN001 : 1,
    PATTERN010 : 2,
    PATTERN011 : 3,
    PATTERN100 : 4,
    PATTERN101 : 5,
    PATTERN110 : 6,
    PATTERN111 : 7
  };

  //---------------------------------------------------------------------
  // QRUtil
  //---------------------------------------------------------------------

  var QRUtil = function() {

    var PATTERN_POSITION_TABLE = [
      [],
      [6, 18],
      [6, 22],
      [6, 26],
      [6, 30],
      [6, 34],
      [6, 22, 38],
      [6, 24, 42],
      [6, 26, 46],
      [6, 28, 50],
      [6, 30, 54],
      [6, 32, 58],
      [6, 34, 62],
      [6, 26, 46, 66],
      [6, 26, 48, 70],
      [6, 26, 50, 74],
      [6, 30, 54, 78],
      [6, 30, 56, 82],
      [6, 30, 58, 86],
      [6, 34, 62, 90],
      [6, 28, 50, 72, 94],
      [6, 26, 50, 74, 98],
      [6, 30, 54, 78, 102],
      [6, 28, 54, 80, 106],
      [6, 32, 58, 84, 110],
      [6, 30, 58, 86, 114],
      [6, 34, 62, 90, 118],
      [6, 26, 50, 74, 98, 122],
      [6, 30, 54, 78, 102, 126],
      [6, 26, 52, 78, 104, 130],
      [6, 30, 56, 82, 108, 134],
      [6, 34, 60, 86, 112, 138],
      [6, 30, 58, 86, 114, 142],
      [6, 34, 62, 90, 118, 146],
      [6, 30, 54, 78, 102, 126, 150],
      [6, 24, 50, 76, 102, 128, 154],
      [6, 28, 54, 80, 106, 132, 158],
      [6, 32, 58, 84, 110, 136, 162],
      [6, 26, 54, 82, 110, 138, 166],
      [6, 30, 58, 86, 114, 142, 170]
    ];
    var G15 = (1 &lt;&lt; 10) | (1 &lt;&lt; 8) | (1 &lt;&lt; 5) | (1 &lt;&lt; 4) | (1 &lt;&lt; 2) | (1 &lt;&lt; 1) | (1 &lt;&lt; 0);
    var G18 = (1 &lt;&lt; 12) | (1 &lt;&lt; 11) | (1 &lt;&lt; 10) | (1 &lt;&lt; 9) | (1 &lt;&lt; 8) | (1 &lt;&lt; 5) | (1 &lt;&lt; 2) | (1 &lt;&lt; 0);
    var G15_MASK = (1 &lt;&lt; 14) | (1 &lt;&lt; 12) | (1 &lt;&lt; 10) | (1 &lt;&lt; 4) | (1 &lt;&lt; 1);

    var _this = {};

    var getBCHDigit = function(data) {
      var digit = 0;
      while (data != 0) {
        digit += 1;
        data &gt;&gt;&gt;= 1;
      }
      return digit;
    };

    _this.getBCHTypeInfo = function(data) {
      var d = data &lt;&lt; 10;
      while (getBCHDigit(d) - getBCHDigit(G15) &gt;= 0) {
        d ^= (G15 &lt;&lt; (getBCHDigit(d) - getBCHDigit(G15) ) );
      }
      return ( (data &lt;&lt; 10) | d) ^ G15_MASK;
    };

    _this.getBCHTypeNumber = function(data) {
      var d = data &lt;&lt; 12;
      while (getBCHDigit(d) - getBCHDigit(G18) &gt;= 0) {
        d ^= (G18 &lt;&lt; (getBCHDigit(d) - getBCHDigit(G18) ) );
      }
      return (data &lt;&lt; 12) | d;
    };

    _this.getPatternPosition = function(typeNumber) {
      return PATTERN_POSITION_TABLE[typeNumber - 1];
    };

    _this.getMaskFunction = function(maskPattern) {

      switch (maskPattern) {

      case QRMaskPattern.PATTERN000 :
        return function(i, j) { return (i + j) % 2 == 0; };
      case QRMaskPattern.PATTERN001 :
        return function(i, j) { return i % 2 == 0; };
      case QRMaskPattern.PATTERN010 :
        return function(i, j) { return j % 3 == 0; };
      case QRMaskPattern.PATTERN011 :
        return function(i, j) { return (i + j) % 3 == 0; };
      case QRMaskPattern.PATTERN100 :
        return function(i, j) { return (Math.floor(i / 2) + Math.floor(j / 3) ) % 2 == 0; };
      case QRMaskPattern.PATTERN101 :
        return function(i, j) { return (i * j) % 2 + (i * j) % 3 == 0; };
      case QRMaskPattern.PATTERN110 :
        return function(i, j) { return ( (i * j) % 2 + (i * j) % 3) % 2 == 0; };
      case QRMaskPattern.PATTERN111 :
        return function(i, j) { return ( (i * j) % 3 + (i + j) % 2) % 2 == 0; };

      default :
        throw 'bad maskPattern:' + maskPattern;
      }
    };

    _this.getErrorCorrectPolynomial = function(errorCorrectLength) {
      var a = qrPolynomial([1], 0);
      for (var i = 0; i &lt; errorCorrectLength; i += 1) {
        a = a.multiply(qrPolynomial([1, QRMath.gexp(i)], 0) );
      }
      return a;
    };

    _this.getLengthInBits = function(mode, type) {

      if (1 &lt;= type &amp;&amp; type &lt; 10) {

        // 1 - 9

        switch(mode) {
        case QRMode.MODE_NUMBER    : return 10;
        case QRMode.MODE_ALPHA_NUM : return 9;
        case QRMode.MODE_8BIT_BYTE : return 8;
        case QRMode.MODE_KANJI     : return 8;
        default :
          throw 'mode:' + mode;
        }

      } else if (type &lt; 27) {

        // 10 - 26

        switch(mode) {
        case QRMode.MODE_NUMBER    : return 12;
        case QRMode.MODE_ALPHA_NUM : return 11;
        case QRMode.MODE_8BIT_BYTE : return 16;
        case QRMode.MODE_KANJI     : return 10;
        default :
          throw 'mode:' + mode;
        }

      } else if (type &lt; 41) {

        // 27 - 40

        switch(mode) {
        case QRMode.MODE_NUMBER    : return 14;
        case QRMode.MODE_ALPHA_NUM : return 13;
        case QRMode.MODE_8BIT_BYTE : return 16;
        case QRMode.MODE_KANJI     : return 12;
        default :
          throw 'mode:' + mode;
        }

      } else {
        throw 'type:' + type;
      }
    };

    _this.getLostPoint = function(qrcode) {

      var moduleCount = qrcode.getModuleCount();

      var lostPoint = 0;

      // LEVEL1

      for (var row = 0; row &lt; moduleCount; row += 1) {
        for (var col = 0; col &lt; moduleCount; col += 1) {

          var sameCount = 0;
          var dark = qrcode.isDark(row, col);

          for (var r = -1; r &lt;= 1; r += 1) {

            if (row + r &lt; 0 || moduleCount &lt;= row + r) {
              continue;
            }

            for (var c = -1; c &lt;= 1; c += 1) {

              if (col + c &lt; 0 || moduleCount &lt;= col + c) {
                continue;
              }

              if (r == 0 &amp;&amp; c == 0) {
                continue;
              }

              if (dark == qrcode.isDark(row + r, col + c) ) {
                sameCount += 1;
              }
            }
          }

          if (sameCount &gt; 5) {
            lostPoint += (3 + sameCount - 5);
          }
        }
      };

      // LEVEL2

      for (var row = 0; row &lt; moduleCount - 1; row += 1) {
        for (var col = 0; col &lt; moduleCount - 1; col += 1) {
          var count = 0;
          if (qrcode.isDark(row, col) ) count += 1;
          if (qrcode.isDark(row + 1, col) ) count += 1;
          if (qrcode.isDark(row, col + 1) ) count += 1;
          if (qrcode.isDark(row + 1, col + 1) ) count += 1;
          if (count == 0 || count == 4) {
            lostPoint += 3;
          }
        }
      }

      // LEVEL3

      for (var row = 0; row &lt; moduleCount; row += 1) {
        for (var col = 0; col &lt; moduleCount - 6; col += 1) {
          if (qrcode.isDark(row, col)
              &amp;&amp; !qrcode.isDark(row, col + 1)
              &amp;&amp;  qrcode.isDark(row, col + 2)
              &amp;&amp;  qrcode.isDark(row, col + 3)
              &amp;&amp;  qrcode.isDark(row, col + 4)
              &amp;&amp; !qrcode.isDark(row, col + 5)
              &amp;&amp;  qrcode.isDark(row, col + 6) ) {
            lostPoint += 40;
          }
        }
      }

      for (var col = 0; col &lt; moduleCount; col += 1) {
        for (var row = 0; row &lt; moduleCount - 6; row += 1) {
          if (qrcode.isDark(row, col)
              &amp;&amp; !qrcode.isDark(row + 1, col)
              &amp;&amp;  qrcode.isDark(row + 2, col)
              &amp;&amp;  qrcode.isDark(row + 3, col)
              &amp;&amp;  qrcode.isDark(row + 4, col)
              &amp;&amp; !qrcode.isDark(row + 5, col)
              &amp;&amp;  qrcode.isDark(row + 6, col) ) {
            lostPoint += 40;
          }
        }
      }

      // LEVEL4

      var darkCount = 0;

      for (var col = 0; col &lt; moduleCount; col += 1) {
        for (var row = 0; row &lt; moduleCount; row += 1) {
          if (qrcode.isDark(row, col) ) {
            darkCount += 1;
          }
        }
      }

      var ratio = Math.abs(100 * darkCount / moduleCount / moduleCount - 50) / 5;
      lostPoint += ratio * 10;

      return lostPoint;
    };

    return _this;
  }();

  //---------------------------------------------------------------------
  // QRMath
  //---------------------------------------------------------------------

  var QRMath = function() {

    var EXP_TABLE = new Array(256);
    var LOG_TABLE = new Array(256);

    // initialize tables
    for (var i = 0; i &lt; 8; i += 1) {
      EXP_TABLE[i] = 1 &lt;&lt; i;
    }
    for (var i = 8; i &lt; 256; i += 1) {
      EXP_TABLE[i] = EXP_TABLE[i - 4]
        ^ EXP_TABLE[i - 5]
        ^ EXP_TABLE[i - 6]
        ^ EXP_TABLE[i - 8];
    }
    for (var i = 0; i &lt; 255; i += 1) {
      LOG_TABLE[EXP_TABLE[i] ] = i;
    }

    var _this = {};

    _this.glog = function(n) {

      if (n &lt; 1) {
        throw 'glog(' + n + ')';
      }

      return LOG_TABLE[n];
    };

    _this.gexp = function(n) {

      while (n &lt; 0) {
        n += 255;
      }

      while (n &gt;= 256) {
        n -= 255;
      }

      return EXP_TABLE[n];
    };

    return _this;
  }();

  //---------------------------------------------------------------------
  // qrPolynomial
  //---------------------------------------------------------------------

  function qrPolynomial(num, shift) {

    if (typeof num.length == 'undefined') {
      throw num.length + '/' + shift;
    }

    var _num = function() {
      var offset = 0;
      while (offset &lt; num.length &amp;&amp; num[offset] == 0) {
        offset += 1;
      }
      var _num = new Array(num.length - offset + shift);
      for (var i = 0; i &lt; num.length - offset; i += 1) {
        _num[i] = num[i + offset];
      }
      return _num;
    }();

    var _this = {};

    _this.getAt = function(index) {
      return _num[index];
    };

    _this.getLength = function() {
      return _num.length;
    };

    _this.multiply = function(e) {

      var num = new Array(_this.getLength() + e.getLength() - 1);

      for (var i = 0; i &lt; _this.getLength(); i += 1) {
        for (var j = 0; j &lt; e.getLength(); j += 1) {
          num[i + j] ^= QRMath.gexp(QRMath.glog(_this.getAt(i) ) + QRMath.glog(e.getAt(j) ) );
        }
      }

      return qrPolynomial(num, 0);
    };

    _this.mod = function(e) {

      if (_this.getLength() - e.getLength() &lt; 0) {
        return _this;
      }

      var ratio = QRMath.glog(_this.getAt(0) ) - QRMath.glog(e.getAt(0) );

      var num = new Array(_this.getLength() );
      for (var i = 0; i &lt; _this.getLength(); i += 1) {
        num[i] = _this.getAt(i);
      }

      for (var i = 0; i &lt; e.getLength(); i += 1) {
        num[i] ^= QRMath.gexp(QRMath.glog(e.getAt(i) ) + ratio);
      }

      // recursive call
      return qrPolynomial(num, 0).mod(e);
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // QRRSBlock
  //---------------------------------------------------------------------

  var QRRSBlock = function() {

    var RS_BLOCK_TABLE = [

      // L
      // M
      // Q
      // H

      // 1
      [1, 26, 19],
      [1, 26, 16],
      [1, 26, 13],
      [1, 26, 9],

      // 2
      [1, 44, 34],
      [1, 44, 28],
      [1, 44, 22],
      [1, 44, 16],

      // 3
      [1, 70, 55],
      [1, 70, 44],
      [2, 35, 17],
      [2, 35, 13],

      // 4
      [1, 100, 80],
      [2, 50, 32],
      [2, 50, 24],
      [4, 25, 9],

      // 5
      [1, 134, 108],
      [2, 67, 43],
      [2, 33, 15, 2, 34, 16],
      [2, 33, 11, 2, 34, 12],

      // 6
      [2, 86, 68],
      [4, 43, 27],
      [4, 43, 19],
      [4, 43, 15],

      // 7
      [2, 98, 78],
      [4, 49, 31],
      [2, 32, 14, 4, 33, 15],
      [4, 39, 13, 1, 40, 14],

      // 8
      [2, 121, 97],
      [2, 60, 38, 2, 61, 39],
      [4, 40, 18, 2, 41, 19],
      [4, 40, 14, 2, 41, 15],

      // 9
      [2, 146, 116],
      [3, 58, 36, 2, 59, 37],
      [4, 36, 16, 4, 37, 17],
      [4, 36, 12, 4, 37, 13],

      // 10
      [2, 86, 68, 2, 87, 69],
      [4, 69, 43, 1, 70, 44],
      [6, 43, 19, 2, 44, 20],
      [6, 43, 15, 2, 44, 16],

      // 11
      [4, 101, 81],
      [1, 80, 50, 4, 81, 51],
      [4, 50, 22, 4, 51, 23],
      [3, 36, 12, 8, 37, 13],

      // 12
      [2, 116, 92, 2, 117, 93],
      [6, 58, 36, 2, 59, 37],
      [4, 46, 20, 6, 47, 21],
      [7, 42, 14, 4, 43, 15],

      // 13
      [4, 133, 107],
      [8, 59, 37, 1, 60, 38],
      [8, 44, 20, 4, 45, 21],
      [12, 33, 11, 4, 34, 12],

      // 14
      [3, 145, 115, 1, 146, 116],
      [4, 64, 40, 5, 65, 41],
      [11, 36, 16, 5, 37, 17],
      [11, 36, 12, 5, 37, 13],

      // 15
      [5, 109, 87, 1, 110, 88],
      [5, 65, 41, 5, 66, 42],
      [5, 54, 24, 7, 55, 25],
      [11, 36, 12, 7, 37, 13],

      // 16
      [5, 122, 98, 1, 123, 99],
      [7, 73, 45, 3, 74, 46],
      [15, 43, 19, 2, 44, 20],
      [3, 45, 15, 13, 46, 16],

      // 17
      [1, 135, 107, 5, 136, 108],
      [10, 74, 46, 1, 75, 47],
      [1, 50, 22, 15, 51, 23],
      [2, 42, 14, 17, 43, 15],

      // 18
      [5, 150, 120, 1, 151, 121],
      [9, 69, 43, 4, 70, 44],
      [17, 50, 22, 1, 51, 23],
      [2, 42, 14, 19, 43, 15],

      // 19
      [3, 141, 113, 4, 142, 114],
      [3, 70, 44, 11, 71, 45],
      [17, 47, 21, 4, 48, 22],
      [9, 39, 13, 16, 40, 14],

      // 20
      [3, 135, 107, 5, 136, 108],
      [3, 67, 41, 13, 68, 42],
      [15, 54, 24, 5, 55, 25],
      [15, 43, 15, 10, 44, 16],

      // 21
      [4, 144, 116, 4, 145, 117],
      [17, 68, 42],
      [17, 50, 22, 6, 51, 23],
      [19, 46, 16, 6, 47, 17],

      // 22
      [2, 139, 111, 7, 140, 112],
      [17, 74, 46],
      [7, 54, 24, 16, 55, 25],
      [34, 37, 13],

      // 23
      [4, 151, 121, 5, 152, 122],
      [4, 75, 47, 14, 76, 48],
      [11, 54, 24, 14, 55, 25],
      [16, 45, 15, 14, 46, 16],

      // 24
      [6, 147, 117, 4, 148, 118],
      [6, 73, 45, 14, 74, 46],
      [11, 54, 24, 16, 55, 25],
      [30, 46, 16, 2, 47, 17],

      // 25
      [8, 132, 106, 4, 133, 107],
      [8, 75, 47, 13, 76, 48],
      [7, 54, 24, 22, 55, 25],
      [22, 45, 15, 13, 46, 16],

      // 26
      [10, 142, 114, 2, 143, 115],
      [19, 74, 46, 4, 75, 47],
      [28, 50, 22, 6, 51, 23],
      [33, 46, 16, 4, 47, 17],

      // 27
      [8, 152, 122, 4, 153, 123],
      [22, 73, 45, 3, 74, 46],
      [8, 53, 23, 26, 54, 24],
      [12, 45, 15, 28, 46, 16],

      // 28
      [3, 147, 117, 10, 148, 118],
      [3, 73, 45, 23, 74, 46],
      [4, 54, 24, 31, 55, 25],
      [11, 45, 15, 31, 46, 16],

      // 29
      [7, 146, 116, 7, 147, 117],
      [21, 73, 45, 7, 74, 46],
      [1, 53, 23, 37, 54, 24],
      [19, 45, 15, 26, 46, 16],

      // 30
      [5, 145, 115, 10, 146, 116],
      [19, 75, 47, 10, 76, 48],
      [15, 54, 24, 25, 55, 25],
      [23, 45, 15, 25, 46, 16],

      // 31
      [13, 145, 115, 3, 146, 116],
      [2, 74, 46, 29, 75, 47],
      [42, 54, 24, 1, 55, 25],
      [23, 45, 15, 28, 46, 16],

      // 32
      [17, 145, 115],
      [10, 74, 46, 23, 75, 47],
      [10, 54, 24, 35, 55, 25],
      [19, 45, 15, 35, 46, 16],

      // 33
      [17, 145, 115, 1, 146, 116],
      [14, 74, 46, 21, 75, 47],
      [29, 54, 24, 19, 55, 25],
      [11, 45, 15, 46, 46, 16],

      // 34
      [13, 145, 115, 6, 146, 116],
      [14, 74, 46, 23, 75, 47],
      [44, 54, 24, 7, 55, 25],
      [59, 46, 16, 1, 47, 17],

      // 35
      [12, 151, 121, 7, 152, 122],
      [12, 75, 47, 26, 76, 48],
      [39, 54, 24, 14, 55, 25],
      [22, 45, 15, 41, 46, 16],

      // 36
      [6, 151, 121, 14, 152, 122],
      [6, 75, 47, 34, 76, 48],
      [46, 54, 24, 10, 55, 25],
      [2, 45, 15, 64, 46, 16],

      // 37
      [17, 152, 122, 4, 153, 123],
      [29, 74, 46, 14, 75, 47],
      [49, 54, 24, 10, 55, 25],
      [24, 45, 15, 46, 46, 16],

      // 38
      [4, 152, 122, 18, 153, 123],
      [13, 74, 46, 32, 75, 47],
      [48, 54, 24, 14, 55, 25],
      [42, 45, 15, 32, 46, 16],

      // 39
      [20, 147, 117, 4, 148, 118],
      [40, 75, 47, 7, 76, 48],
      [43, 54, 24, 22, 55, 25],
      [10, 45, 15, 67, 46, 16],

      // 40
      [19, 148, 118, 6, 149, 119],
      [18, 75, 47, 31, 76, 48],
      [34, 54, 24, 34, 55, 25],
      [20, 45, 15, 61, 46, 16]
    ];

    var qrRSBlock = function(totalCount, dataCount) {
      var _this = {};
      _this.totalCount = totalCount;
      _this.dataCount = dataCount;
      return _this;
    };

    var _this = {};

    var getRsBlockTable = function(typeNumber, errorCorrectionLevel) {

      switch(errorCorrectionLevel) {
      case QRErrorCorrectionLevel.L :
        return RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 0];
      case QRErrorCorrectionLevel.M :
        return RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 1];
      case QRErrorCorrectionLevel.Q :
        return RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 2];
      case QRErrorCorrectionLevel.H :
        return RS_BLOCK_TABLE[(typeNumber - 1) * 4 + 3];
      default :
        return undefined;
      }
    };

    _this.getRSBlocks = function(typeNumber, errorCorrectionLevel) {

      var rsBlock = getRsBlockTable(typeNumber, errorCorrectionLevel);

      if (typeof rsBlock == 'undefined') {
        throw 'bad rs block @ typeNumber:' + typeNumber +
            '/errorCorrectionLevel:' + errorCorrectionLevel;
      }

      var length = rsBlock.length / 3;

      var list = [];

      for (var i = 0; i &lt; length; i += 1) {

        var count = rsBlock[i * 3 + 0];
        var totalCount = rsBlock[i * 3 + 1];
        var dataCount = rsBlock[i * 3 + 2];

        for (var j = 0; j &lt; count; j += 1) {
          list.push(qrRSBlock(totalCount, dataCount) );
        }
      }

      return list;
    };

    return _this;
  }();

  //---------------------------------------------------------------------
  // qrBitBuffer
  //---------------------------------------------------------------------

  var qrBitBuffer = function() {

    var _buffer = [];
    var _length = 0;

    var _this = {};

    _this.getBuffer = function() {
      return _buffer;
    };

    _this.getAt = function(index) {
      var bufIndex = Math.floor(index / 8);
      return ( (_buffer[bufIndex] &gt;&gt;&gt; (7 - index % 8) ) &amp; 1) == 1;
    };

    _this.put = function(num, length) {
      for (var i = 0; i &lt; length; i += 1) {
        _this.putBit( ( (num &gt;&gt;&gt; (length - i - 1) ) &amp; 1) == 1);
      }
    };

    _this.getLengthInBits = function() {
      return _length;
    };

    _this.putBit = function(bit) {

      var bufIndex = Math.floor(_length / 8);
      if (_buffer.length &lt;= bufIndex) {
        _buffer.push(0);
      }

      if (bit) {
        _buffer[bufIndex] |= (0x80 &gt;&gt;&gt; (_length % 8) );
      }

      _length += 1;
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // qrNumber
  //---------------------------------------------------------------------

  var qrNumber = function(data) {

    var _mode = QRMode.MODE_NUMBER;
    var _data = data;

    var _this = {};

    _this.getMode = function() {
      return _mode;
    };

    _this.getLength = function(buffer) {
      return _data.length;
    };

    _this.write = function(buffer) {

      var data = _data;

      var i = 0;

      while (i + 2 &lt; data.length) {
        buffer.put(strToNum(data.substring(i, i + 3) ), 10);
        i += 3;
      }

      if (i &lt; data.length) {
        if (data.length - i == 1) {
          buffer.put(strToNum(data.substring(i, i + 1) ), 4);
        } else if (data.length - i == 2) {
          buffer.put(strToNum(data.substring(i, i + 2) ), 7);
        }
      }
    };

    var strToNum = function(s) {
      var num = 0;
      for (var i = 0; i &lt; s.length; i += 1) {
        num = num * 10 + chatToNum(s.charAt(i) );
      }
      return num;
    };

    var chatToNum = function(c) {
      if ('0' &lt;= c &amp;&amp; c &lt;= '9') {
        return c.charCodeAt(0) - '0'.charCodeAt(0);
      }
      throw 'illegal char :' + c;
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // qrAlphaNum
  //---------------------------------------------------------------------

  var qrAlphaNum = function(data) {

    var _mode = QRMode.MODE_ALPHA_NUM;
    var _data = data;

    var _this = {};

    _this.getMode = function() {
      return _mode;
    };

    _this.getLength = function(buffer) {
      return _data.length;
    };

    _this.write = function(buffer) {

      var s = _data;

      var i = 0;

      while (i + 1 &lt; s.length) {
        buffer.put(
          getCode(s.charAt(i) ) * 45 +
          getCode(s.charAt(i + 1) ), 11);
        i += 2;
      }

      if (i &lt; s.length) {
        buffer.put(getCode(s.charAt(i) ), 6);
      }
    };

    var getCode = function(c) {

      if ('0' &lt;= c &amp;&amp; c &lt;= '9') {
        return c.charCodeAt(0) - '0'.charCodeAt(0);
      } else if ('A' &lt;= c &amp;&amp; c &lt;= 'Z') {
        return c.charCodeAt(0) - 'A'.charCodeAt(0) + 10;
      } else {
        switch (c) {
        case ' ' : return 36;
        case '$' : return 37;
        case '%' : return 38;
        case '*' : return 39;
        case '+' : return 40;
        case '-' : return 41;
        case '.' : return 42;
        case '/' : return 43;
        case ':' : return 44;
        default :
          throw 'illegal char :' + c;
        }
      }
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // qr8BitByte
  //---------------------------------------------------------------------

  var qr8BitByte = function(data) {

    var _mode = QRMode.MODE_8BIT_BYTE;
    var _data = data;
    var _bytes = qrcode.stringToBytes(data);

    var _this = {};

    _this.getMode = function() {
      return _mode;
    };

    _this.getLength = function(buffer) {
      return _bytes.length;
    };

    _this.write = function(buffer) {
      for (var i = 0; i &lt; _bytes.length; i += 1) {
        buffer.put(_bytes[i], 8);
      }
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // qrKanji
  //---------------------------------------------------------------------

  var qrKanji = function(data) {

    var _mode = QRMode.MODE_KANJI;
    var _data = data;

    var stringToBytes = qrcode.stringToBytesFuncs['SJIS'];
    if (!stringToBytes) {
      throw 'sjis not supported.';
    }
    !function(c, code) {
      // self test for sjis support.
      var test = stringToBytes(c);
      if (test.length != 2 || ( (test[0] &lt;&lt; 8) | test[1]) != code) {
        throw 'sjis not supported.';
      }
    }('\u53cb', 0x9746);

    var _bytes = stringToBytes(data);

    var _this = {};

    _this.getMode = function() {
      return _mode;
    };

    _this.getLength = function(buffer) {
      return ~~(_bytes.length / 2);
    };

    _this.write = function(buffer) {

      var data = _bytes;

      var i = 0;

      while (i + 1 &lt; data.length) {

        var c = ( (0xff &amp; data[i]) &lt;&lt; 8) | (0xff &amp; data[i + 1]);

        if (0x8140 &lt;= c &amp;&amp; c &lt;= 0x9FFC) {
          c -= 0x8140;
        } else if (0xE040 &lt;= c &amp;&amp; c &lt;= 0xEBBF) {
          c -= 0xC140;
        } else {
          throw 'illegal char at ' + (i + 1) + '/' + c;
        }

        c = ( (c &gt;&gt;&gt; 8) &amp; 0xff) * 0xC0 + (c &amp; 0xff);

        buffer.put(c, 13);

        i += 2;
      }

      if (i &lt; data.length) {
        throw 'illegal char at ' + (i + 1);
      }
    };

    return _this;
  };

  //=====================================================================
  // GIF Support etc.
  //

  //---------------------------------------------------------------------
  // byteArrayOutputStream
  //---------------------------------------------------------------------

  var byteArrayOutputStream = function() {

    var _bytes = [];

    var _this = {};

    _this.writeByte = function(b) {
      _bytes.push(b &amp; 0xff);
    };

    _this.writeShort = function(i) {
      _this.writeByte(i);
      _this.writeByte(i &gt;&gt;&gt; 8);
    };

    _this.writeBytes = function(b, off, len) {
      off = off || 0;
      len = len || b.length;
      for (var i = 0; i &lt; len; i += 1) {
        _this.writeByte(b[i + off]);
      }
    };

    _this.writeString = function(s) {
      for (var i = 0; i &lt; s.length; i += 1) {
        _this.writeByte(s.charCodeAt(i) );
      }
    };

    _this.toByteArray = function() {
      return _bytes;
    };

    _this.toString = function() {
      var s = '';
      s += '[';
      for (var i = 0; i &lt; _bytes.length; i += 1) {
        if (i &gt; 0) {
          s += ',';
        }
        s += _bytes[i];
      }
      s += ']';
      return s;
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // base64EncodeOutputStream
  //---------------------------------------------------------------------

  var base64EncodeOutputStream = function() {

    var _buffer = 0;
    var _buflen = 0;
    var _length = 0;
    var _base64 = '';

    var _this = {};

    var writeEncoded = function(b) {
      _base64 += String.fromCharCode(encode(b &amp; 0x3f) );
    };

    var encode = function(n) {
      if (n &lt; 0) {
        // error.
      } else if (n &lt; 26) {
        return 0x41 + n;
      } else if (n &lt; 52) {
        return 0x61 + (n - 26);
      } else if (n &lt; 62) {
        return 0x30 + (n - 52);
      } else if (n == 62) {
        return 0x2b;
      } else if (n == 63) {
        return 0x2f;
      }
      throw 'n:' + n;
    };

    _this.writeByte = function(n) {

      _buffer = (_buffer &lt;&lt; 8) | (n &amp; 0xff);
      _buflen += 8;
      _length += 1;

      while (_buflen &gt;= 6) {
        writeEncoded(_buffer &gt;&gt;&gt; (_buflen - 6) );
        _buflen -= 6;
      }
    };

    _this.flush = function() {

      if (_buflen &gt; 0) {
        writeEncoded(_buffer &lt;&lt; (6 - _buflen) );
        _buffer = 0;
        _buflen = 0;
      }

      if (_length % 3 != 0) {
        // padding
        var padlen = 3 - _length % 3;
        for (var i = 0; i &lt; padlen; i += 1) {
          _base64 += '=';
        }
      }
    };

    _this.toString = function() {
      return _base64;
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // base64DecodeInputStream
  //---------------------------------------------------------------------

  var base64DecodeInputStream = function(str) {

    var _str = str;
    var _pos = 0;
    var _buffer = 0;
    var _buflen = 0;

    var _this = {};

    _this.read = function() {

      while (_buflen &lt; 8) {

        if (_pos &gt;= _str.length) {
          if (_buflen == 0) {
            return -1;
          }
          throw 'unexpected end of file./' + _buflen;
        }

        var c = _str.charAt(_pos);
        _pos += 1;

        if (c == '=') {
          _buflen = 0;
          return -1;
        } else if (c.match(/^\s$/) ) {
          // ignore if whitespace.
          continue;
        }

        _buffer = (_buffer &lt;&lt; 6) | decode(c.charCodeAt(0) );
        _buflen += 6;
      }

      var n = (_buffer &gt;&gt;&gt; (_buflen - 8) ) &amp; 0xff;
      _buflen -= 8;
      return n;
    };

    var decode = function(c) {
      if (0x41 &lt;= c &amp;&amp; c &lt;= 0x5a) {
        return c - 0x41;
      } else if (0x61 &lt;= c &amp;&amp; c &lt;= 0x7a) {
        return c - 0x61 + 26;
      } else if (0x30 &lt;= c &amp;&amp; c &lt;= 0x39) {
        return c - 0x30 + 52;
      } else if (c == 0x2b) {
        return 62;
      } else if (c == 0x2f) {
        return 63;
      } else {
        throw 'c:' + c;
      }
    };

    return _this;
  };

  //---------------------------------------------------------------------
  // gifImage (B/W)
  //---------------------------------------------------------------------

  var gifImage = function(width, height) {

    var _width = width;
    var _height = height;
    var _data = new Array(width * height);

    var _this = {};

    _this.setPixel = function(x, y, pixel) {
      _data[y * _width + x] = pixel;
    };

    _this.write = function(out) {

      //---------------------------------
      // GIF Signature

      out.writeString('GIF87a');

      //---------------------------------
      // Screen Descriptor

      out.writeShort(_width);
      out.writeShort(_height);

      out.writeByte(0x80); // 2bit
      out.writeByte(0);
      out.writeByte(0);

      //---------------------------------
      // Global Color Map

      // black
      out.writeByte(0x00);
      out.writeByte(0x00);
      out.writeByte(0x00);

      // white
      out.writeByte(0xff);
      out.writeByte(0xff);
      out.writeByte(0xff);

      //---------------------------------
      // Image Descriptor

      out.writeString(',');
      out.writeShort(0);
      out.writeShort(0);
      out.writeShort(_width);
      out.writeShort(_height);
      out.writeByte(0);

      //---------------------------------
      // Local Color Map

      //---------------------------------
      // Raster Data

      var lzwMinCodeSize = 2;
      var raster = getLZWRaster(lzwMinCodeSize);

      out.writeByte(lzwMinCodeSize);

      var offset = 0;

      while (raster.length - offset &gt; 255) {
        out.writeByte(255);
        out.writeBytes(raster, offset, 255);
        offset += 255;
      }

      out.writeByte(raster.length - offset);
      out.writeBytes(raster, offset, raster.length - offset);
      out.writeByte(0x00);

      //---------------------------------
      // GIF Terminator
      out.writeString(';');
    };

    var bitOutputStream = function(out) {

      var _out = out;
      var _bitLength = 0;
      var _bitBuffer = 0;

      var _this = {};

      _this.write = function(data, length) {

        if ( (data &gt;&gt;&gt; length) != 0) {
          throw 'length over';
        }

        while (_bitLength + length &gt;= 8) {
          _out.writeByte(0xff &amp; ( (data &lt;&lt; _bitLength) | _bitBuffer) );
          length -= (8 - _bitLength);
          data &gt;&gt;&gt;= (8 - _bitLength);
          _bitBuffer = 0;
          _bitLength = 0;
        }

        _bitBuffer = (data &lt;&lt; _bitLength) | _bitBuffer;
        _bitLength = _bitLength + length;
      };

      _this.flush = function() {
        if (_bitLength &gt; 0) {
          _out.writeByte(_bitBuffer);
        }
      };

      return _this;
    };

    var getLZWRaster = function(lzwMinCodeSize) {

      var clearCode = 1 &lt;&lt; lzwMinCodeSize;
      var endCode = (1 &lt;&lt; lzwMinCodeSize) + 1;
      var bitLength = lzwMinCodeSize + 1;

      // Setup LZWTable
      var table = lzwTable();

      for (var i = 0; i &lt; clearCode; i += 1) {
        table.add(String.fromCharCode(i) );
      }
      table.add(String.fromCharCode(clearCode) );
      table.add(String.fromCharCode(endCode) );

      var byteOut = byteArrayOutputStream();
      var bitOut = bitOutputStream(byteOut);

      // clear code
      bitOut.write(clearCode, bitLength);

      var dataIndex = 0;

      var s = String.fromCharCode(_data[dataIndex]);
      dataIndex += 1;

      while (dataIndex &lt; _data.length) {

        var c = String.fromCharCode(_data[dataIndex]);
        dataIndex += 1;

        if (table.contains(s + c) ) {

          s = s + c;

        } else {

          bitOut.write(table.indexOf(s), bitLength);

          if (table.size() &lt; 0xfff) {

            if (table.size() == (1 &lt;&lt; bitLength) ) {
              bitLength += 1;
            }

            table.add(s + c);
          }

          s = c;
        }
      }

      bitOut.write(table.indexOf(s), bitLength);

      // end code
      bitOut.write(endCode, bitLength);

      bitOut.flush();

      return byteOut.toByteArray();
    };

    var lzwTable = function() {

      var _map = {};
      var _size = 0;

      var _this = {};

      _this.add = function(key) {
        if (_this.contains(key) ) {
          throw 'dup key:' + key;
        }
        _map[key] = _size;
        _size += 1;
      };

      _this.size = function() {
        return _size;
      };

      _this.indexOf = function(key) {
        return _map[key];
      };

      _this.contains = function(key) {
        return typeof _map[key] != 'undefined';
      };

      return _this;
    };

    return _this;
  };

  var createDataURL = function(width, height, getPixel) {
    var gif = gifImage(width, height);
    for (var y = 0; y &lt; height; y += 1) {
      for (var x = 0; x &lt; width; x += 1) {
        gif.setPixel(x, y, getPixel(x, y) );
      }
    }

    var b = byteArrayOutputStream();
    gif.write(b);

    var base64 = base64EncodeOutputStream();
    var bytes = b.toByteArray();
    for (var i = 0; i &lt; bytes.length; i += 1) {
      base64.writeByte(bytes[i]);
    }
    base64.flush();

    return 'data:image/gif;base64,' + base64;
  };

  //---------------------------------------------------------------------
  // returns qrcode function.

  return qrcode;
}();

// multibyte support
!function() {

  qrcode.stringToBytesFuncs['UTF-8'] = function(s) {
    // http://stackoverflow.com/questions/18729405/how-to-convert-utf8-string-to-byte-array
    function toUTF8Array(str) {
      var utf8 = [];
      for (var i=0; i &lt; str.length; i++) {
        var charcode = str.charCodeAt(i);
        if (charcode &lt; 0x80) utf8.push(charcode);
        else if (charcode &lt; 0x800) {
          utf8.push(0xc0 | (charcode &gt;&gt; 6),
              0x80 | (charcode &amp; 0x3f));
        }
        else if (charcode &lt; 0xd800 || charcode &gt;= 0xe000) {
          utf8.push(0xe0 | (charcode &gt;&gt; 12),
              0x80 | ((charcode&gt;&gt;6) &amp; 0x3f),
              0x80 | (charcode &amp; 0x3f));
        }
        // surrogate pair
        else {
          i++;
          // UTF-16 encodes 0x10000-0x10FFFF by
          // subtracting 0x10000 and splitting the
          // 20 bits of 0x0-0xFFFFF into two halves
          charcode = 0x10000 + (((charcode &amp; 0x3ff)&lt;&lt;10)
            | (str.charCodeAt(i) &amp; 0x3ff));
          utf8.push(0xf0 | (charcode &gt;&gt;18),
              0x80 | ((charcode&gt;&gt;12) &amp; 0x3f),
              0x80 | ((charcode&gt;&gt;6) &amp; 0x3f),
              0x80 | (charcode &amp; 0x3f));
        }
      }
      return utf8;
    }
    return toUTF8Array(s);
  };

}();

(function (factory) {
  if (typeof define === 'function' &amp;&amp; define.amd) {
      define([], factory);
  } else if (typeof exports === 'object') {
      module.exports = factory();
  }
}(function () {
    return qrcode;
}));

  &lt;/script&gt;
&lt;/head&gt;
&lt;body&gt;
  &lt;!-- Build version marker. Bumped by .0000001 on every source change so
       smoke tests can verify which build is running. Starting from .1000001
       on 2026-06-09. --&gt;
  &lt;div id="app-version"&gt;.1000087&lt;/div&gt;

  &lt;!-- tabs --&gt;
  &lt;div id="tabbar"&gt;
    &lt;button id="mobile-settings-toggle" class="header-mobile-only" aria-label="Open settings"&gt;&amp;#9776;&lt;/button&gt;
    &lt;div class="tab active" data-tab="chat"&gt;Chat&lt;/div&gt;
    &lt;div class="tab" data-tab="settings"&gt;Settings&lt;/div&gt;
    &lt;span id="remote-badge" class="remote-badge" title="" style="display:none;"&gt;📱 0&lt;/span&gt;
  &lt;/div&gt;

  &lt;!-- first-run onboarding wizard. Shown when the user's accepted
       onboarding version differs from kCurrentOnboardingVersion.
       Skippable; bumping the version in C++ replays it for everyone. --&gt;
  &lt;div id="onb-overlay" role="dialog" aria-modal="true"&gt;
    &lt;div class="onb-card"&gt;
      &lt;div class="onb-stepper"&gt;
        &lt;span class="onb-step" data-step="welcome"&gt;1 · Welcome&lt;/span&gt;
        &lt;span class="onb-step" data-step="key"&gt;2 · API key&lt;/span&gt;
        &lt;span class="onb-step" data-step="cap"&gt;3 · Spend cap&lt;/span&gt;
        &lt;span class="onb-step" data-step="model"&gt;4 · Pick a model&lt;/span&gt;
        &lt;span class="onb-step" data-step="done"&gt;5 · Done&lt;/span&gt;
      &lt;/div&gt;

      &lt;div class="onb-pane" data-step="welcome"&gt;
        &lt;h2&gt;Welcome to PracSim ModelerAI&lt;/h2&gt;
        &lt;p&gt;A short walkthrough so you're ready to chat with your FlexSim model. About 3 minutes.&lt;/p&gt;
        &lt;p&gt;&lt;strong&gt;What this is:&lt;/strong&gt; an AI assistant that lives inside FlexSim. You talk to it in plain English about your model — what's in it, what to change, what's broken. It can read your model directly and (with permission) make edits.&lt;/p&gt;
        &lt;p&gt;&lt;strong&gt;What's required:&lt;/strong&gt;&lt;/p&gt;
        &lt;ul&gt;
          &lt;li&gt;An &lt;strong&gt;Anthropic API key&lt;/strong&gt; (you'll set one up in the next step — about 90 seconds).&lt;/li&gt;
          &lt;li&gt;A small budget on Anthropic's side. Typical usage is pennies per conversation; we'll show how to set a spending cap.&lt;/li&gt;
        &lt;/ul&gt;
        &lt;p&gt;Your model contents are sent directly from this computer to Anthropic. PracSim never sees them. Anthropic doesn't train on API data and auto-deletes after 7 days.&lt;/p&gt;
      &lt;/div&gt;

      &lt;div class="onb-pane" data-step="key" hidden&gt;
        &lt;h2&gt;Bring your own API key&lt;/h2&gt;
        &lt;p&gt;ModelerAI uses your own Anthropic account — your key, your budget, your control. PracSim never holds your key.&lt;/p&gt;
        &lt;ol&gt;
          &lt;li&gt;Open &lt;a href="https://console.anthropic.com/" target="_blank" rel="noopener noreferrer"&gt;console.anthropic.com&lt;/a&gt;.&lt;/li&gt;
          &lt;li&gt;Sign in (or create an account — free, no card needed for the signup).&lt;/li&gt;
          &lt;li&gt;Go to &lt;strong&gt;Settings → API Keys → Create Key&lt;/strong&gt;.&lt;/li&gt;
          &lt;li&gt;Copy the key (starts with &lt;code&gt;sk-ant-&lt;/code&gt;) and paste it below.&lt;/li&gt;
        &lt;/ol&gt;
        &lt;div class="onb-keyrow"&gt;
          &lt;input type="password" id="onb-key" placeholder="sk-ant-..." autocomplete="off"&gt;
          &lt;button id="onb-key-save"&gt;Save key&lt;/button&gt;
        &lt;/div&gt;
        &lt;p class="onb-key-status meta" id="onb-key-status"&gt;Your key is DPAPI-encrypted on this Windows account before it touches disk. Plaintext never reaches the viewer after this step.&lt;/p&gt;
        &lt;p class="meta"&gt;You can skip this if you've already configured a key in Settings → Providers, or do it later — the rest of the wizard works without it.&lt;/p&gt;
      &lt;/div&gt;

      &lt;div class="onb-pane" data-step="cap" hidden&gt;
        &lt;h2&gt;Set a spend cap (strongly recommended)&lt;/h2&gt;
        &lt;p&gt;API usage is metered by Anthropic and billed monthly. To avoid surprise bills, set a hard cap on YOUR Anthropic account before you start using ModelerAI for real work.&lt;/p&gt;
        &lt;ol&gt;
          &lt;li&gt;Open &lt;a href="https://console.anthropic.com/settings/limits" target="_blank" rel="noopener noreferrer"&gt;console.anthropic.com → Settings → Limits&lt;/a&gt;.&lt;/li&gt;
          &lt;li&gt;Set a monthly spend cap that matches your comfort level. &lt;strong&gt;$20–$50 / month is plenty for typical FlexSim work&lt;/strong&gt;; raise later if you find yourself bumping it.&lt;/li&gt;
          &lt;li&gt;Anthropic will refuse further API calls automatically once you hit the cap — your card is never charged past it.&lt;/li&gt;
        &lt;/ol&gt;
        &lt;p&gt;ModelerAI also has its own &lt;strong&gt;per-session cost ceiling&lt;/strong&gt; in Settings → Usage (soft warn + hard stop). Belt and suspenders.&lt;/p&gt;
        &lt;label class="onb-ack"&gt;
          &lt;input type="checkbox" id="onb-cap-ack"&gt;
          &lt;span&gt;I have read this and understand I'm responsible for setting a spend cap on my own Anthropic account.&lt;/span&gt;
        &lt;/label&gt;
      &lt;/div&gt;

      &lt;div class="onb-pane" data-step="model" hidden&gt;
        &lt;h2&gt;Pick a starting model&lt;/h2&gt;
        &lt;p&gt;You can switch any time from the model picker at the top of the chat. Quick guide:&lt;/p&gt;
        &lt;div class="onb-model-grid"&gt;
          &lt;div class="onb-model-card"&gt;
            &lt;h4&gt;Haiku 4.5&lt;/h4&gt;
            &lt;div class="meta"&gt;Cheapest, fastest. Good for: simple questions, exploring a model, slash commands.&lt;/div&gt;
            &lt;div class="onb-model-cost"&gt;~$1 / Mtok in · $5 / Mtok out&lt;/div&gt;
            &lt;button class="onb-pick" data-model="claude-haiku-4-5"&gt;Use Haiku&lt;/button&gt;
          &lt;/div&gt;
          &lt;div class="onb-model-card onb-model-card-recommended"&gt;
            &lt;h4&gt;Sonnet 4.6 — Recommended&lt;/h4&gt;
            &lt;div class="meta"&gt;Balanced quality + cost. Default for most work.&lt;/div&gt;
            &lt;div class="onb-model-cost"&gt;~$3 / Mtok in · $15 / Mtok out&lt;/div&gt;
            &lt;button class="onb-pick" data-model="claude-sonnet-4-6"&gt;Use Sonnet&lt;/button&gt;
          &lt;/div&gt;
          &lt;div class="onb-model-card"&gt;
            &lt;h4&gt;Opus 4.7&lt;/h4&gt;
            &lt;div class="meta"&gt;Best reasoning. Slow + expensive. Use for thorny problems.&lt;/div&gt;
            &lt;div class="onb-model-cost"&gt;~$15 / Mtok in · $75 / Mtok out&lt;/div&gt;
            &lt;button class="onb-pick" data-model="claude-opus-4-7"&gt;Use Opus&lt;/button&gt;
          &lt;/div&gt;
        &lt;/div&gt;
        &lt;p class="meta" id="onb-model-current"&gt;Current selection: …&lt;/p&gt;
      &lt;/div&gt;

      &lt;div class="onb-pane" data-step="done" hidden&gt;
        &lt;h2&gt;You're ready&lt;/h2&gt;
        &lt;p&gt;Quick reference for what's next:&lt;/p&gt;
        &lt;ul&gt;
          &lt;li&gt;&lt;strong&gt;Type to chat&lt;/strong&gt; — ask anything about your model.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;Type &lt;code&gt;/&lt;/code&gt;&lt;/strong&gt; to see slash commands.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;Mode selector&lt;/strong&gt; (Ask / Plan / Apply) at the top right controls what the AI is allowed to do. Default: Ask (read-only).&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;Settings tab&lt;/strong&gt; for theme, model, providers, knowledge base, debug, privacy, and the cost ceiling.&lt;/li&gt;
        &lt;/ul&gt;
        &lt;p class="meta"&gt;You can re-open this guide any time by clicking the help icon in Settings → About. We won't show it again on launch unless a major new feature lands.&lt;/p&gt;
      &lt;/div&gt;

      &lt;div class="onb-actions"&gt;
        &lt;button class="onb-skip" id="onb-skip"&gt;Skip the rest&lt;/button&gt;
        &lt;button class="onb-back" id="onb-back" disabled&gt;Back&lt;/button&gt;
        &lt;button class="onb-next" id="onb-next"&gt;Next&lt;/button&gt;
        &lt;button class="onb-finish" id="onb-finish" hidden&gt;Get started&lt;/button&gt;
      &lt;/div&gt;
    &lt;/div&gt;
  &lt;/div&gt;

  &lt;!-- custom-provider acknowledgement modal. Shown when the user clicks
       "Add provider" with a 3rd-party endpoint; explicit checkbox + I-Agree
       required before the add envelope is dispatched. Red variant for
       known-risky providers (currently DeepSeek by base_url match). --&gt;
  &lt;div id="cp-ack-overlay" role="dialog" aria-modal="true"&gt;
    &lt;div class="cp-ack-card" id="cp-ack-card"&gt;
      &lt;h2 id="cp-ack-title"&gt;Custom Provider — Acknowledge Their Terms&lt;/h2&gt;
      &lt;div class="cp-ack-body" id="cp-ack-body"&gt;&lt;/div&gt;
      &lt;label class="cp-ack-confirm-row"&gt;
        &lt;input type="checkbox" id="cp-ack-confirm"&gt;
        &lt;span&gt;I have read this provider's terms of service and privacy policy and accept that they (not PracSim) govern my use of this provider.&lt;/span&gt;
      &lt;/label&gt;
      &lt;div class="cp-ack-actions"&gt;
        &lt;button id="cp-ack-cancel" class="cp-ack-cancel"&gt;Cancel&lt;/button&gt;
        &lt;button id="cp-ack-agree" class="cp-ack-agree" disabled&gt;I Agree — Add Provider&lt;/button&gt;
      &lt;/div&gt;
    &lt;/div&gt;
  &lt;/div&gt;

  &lt;!-- approval modal (mutating-tool ack). Renders above the chat pane
       when a pending tool_approval_request is active. --&gt;
  &lt;div id="approval-overlay"&gt;
    &lt;div class="approval-card"&gt;
      &lt;h3 id="approval-name"&gt;Tool wants to run&lt;/h3&gt;
      &lt;div&gt;&lt;span class="approval-cat" id="approval-cat"&gt;MUTATING&lt;/span&gt;&lt;/div&gt;
      &lt;div class="approval-desc" id="approval-desc"&gt;&lt;/div&gt;
      &lt;div class="approval-args" id="approval-args"&gt;&lt;/div&gt;
      &lt;div class="approval-actions"&gt;
        &lt;button class="deny"    id="approval-deny"&gt;Deny&lt;/button&gt;
        &lt;button class="approve" id="approval-approve"&gt;Approve&lt;/button&gt;
        &lt;button class="always"  id="approval-always"&gt;Always allow this tool&lt;/button&gt;
      &lt;/div&gt;
    &lt;/div&gt;
  &lt;/div&gt;

  &lt;!-- chat pane --&gt;
  &lt;section id="pane-chat" class="pane active"&gt;
    &lt;!-- Floating popovers re-anchored to the composer toolbar at the bottom.
         Kept at the section level so they can absolute-position from any
         button without being clipped by the composer's overflow rules. --&gt;
    &lt;div id="model-picker-popover" role="listbox" aria-label="Pick a model"&gt;&lt;/div&gt;
    &lt;div id="mode-popover" role="dialog" aria-label="Mode"&gt;&lt;/div&gt;
    &lt;!-- Anchor-missing bar: shown when the panel is open but there's no
         in-model ModelerAI object to anchor the session to. The chat
         still works (in-memory only) — nothing persists to disk until
         the user runs /addmodelerai on a model. --&gt;
    &lt;div id="anchor-bar" style="display:none;"&gt;
      &lt;span class="anchor-icon"&gt;⚓&lt;/span&gt;
      &lt;span id="anchor-text"&gt;This conversation isn't anchored to a model — run &lt;code&gt;/addmodelerai&lt;/code&gt; to start saving it.&lt;/span&gt;
    &lt;/div&gt;
    &lt;main id="messages"&gt;
      &lt;div class="placeholder"&gt;
        &lt;strong&gt;PracSim ModelerAI&lt;/strong&gt;
        Ask about your FlexSim model in plain English.&lt;br&gt;
        Type &lt;code&gt;/&lt;/code&gt; to see commands, or &lt;code&gt;/help&lt;/code&gt; for a quick tour.
      &lt;/div&gt;
    &lt;/main&gt;
    &lt;!-- Debug drawer — only visible when Settings -&gt; General -&gt; Show debug info is ON.
         Populated by `debug_turn` state_updates the agent pushes at the start of each turn. --&gt;
    &lt;div id="debug-drawer" style="display:none;"&gt;
      &lt;div class="debug-head" id="debug-head"&gt;
        &lt;span&gt;Debug&lt;/span&gt;
        &lt;span class="debug-meta" id="debug-meta"&gt;no turn yet&lt;/span&gt;
        &lt;button class="debug-collapse" id="debug-collapse"&gt;−&lt;/button&gt;
      &lt;/div&gt;
      &lt;div class="debug-body" id="debug-body"&gt;
        &lt;h4&gt;System prompt (stable / cached)&lt;/h4&gt;
        &lt;pre id="debug-sp-stable"&gt;(empty)&lt;/pre&gt;
        &lt;h4&gt;System prompt (variable / suffix)&lt;/h4&gt;
        &lt;pre id="debug-sp-var"&gt;(empty)&lt;/pre&gt;
        &lt;h4&gt;Last turn&lt;/h4&gt;
        &lt;pre id="debug-last-turn"&gt;(empty)&lt;/pre&gt;
      &lt;/div&gt;
    &lt;/div&gt;
    &lt;!-- Drag bar to resize the composer textarea up/down. Lives above the
         composer-wrap so it sits between the messages area and the input. --&gt;
    &lt;div id="composer-splitter" title="Drag to resize the input area"&gt;&lt;/div&gt;
    &lt;div id="composer-wrap"&gt;
      &lt;!-- "Still thinking" indicator. Hidden by default; .show flips it
           visible. The pulsing dot animation is CSS-only so it keeps
           moving even if the JS thread is wedged on a long parse — the
           one signal we MUST give the user that the page isn't frozen. --&gt;
      &lt;div id="thinking-indicator" class="thinking-indicator" aria-live="polite"&gt;
        &lt;span class="thinking-dot"&gt;&lt;/span&gt;
        &lt;span id="thinking-label" class="thinking-label"&gt;Thinking…&lt;/span&gt;
        &lt;span id="thinking-elapsed" class="thinking-elapsed"&gt;0s&lt;/span&gt;
      &lt;/div&gt;
      &lt;div id="save-indicator" class="save-indicator"&gt;&lt;/div&gt;
      &lt;div id="cost-warning-banner" style="display:none;"&gt;&lt;/div&gt;
      &lt;div id="composer-toast" role="status" aria-live="polite"&gt;&lt;/div&gt;
      &lt;div id="slash-popover" role="listbox" aria-label="Slash commands"&gt;&lt;/div&gt;
      &lt;form id="composer"&gt;
        &lt;!-- Image attachments appear here as compact chips. Hidden when
             the list is empty (CSS :empty rule on #attachments-row). --&gt;
        &lt;div id="attachments-row" aria-label="Attachments"&gt;&lt;/div&gt;
        &lt;!-- Hidden file input — opened by the + button. --&gt;
        &lt;input type="file" id="attach-input" accept="image/*" multiple style="display:none;"&gt;
        &lt;textarea id="input" rows="1" placeholder="Ask about your model... (type / for commands)"&gt;&lt;/textarea&gt;
        &lt;div id="composer-actions"&gt;
          &lt;div id="composer-actions-left"&gt;
            &lt;button type="button" class="composer-action-btn" id="attach-btn" title="Attach images (click, paste, or drag-drop)"&gt;
              &lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;
                &lt;path d="M12 5v14M5 12h14"/&gt;
              &lt;/svg&gt;
            &lt;/button&gt;
            &lt;button type="button" class="composer-action-btn" id="slash-btn" title="Slash commands (/ in input)"&gt;
              &lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;
                &lt;path d="M16 5L8 19"/&gt;
              &lt;/svg&gt;
            &lt;/button&gt;
            &lt;button type="button" class="composer-toolbar-btn" id="model-btn" title="Switch model"&gt;
              &lt;span class="tb-icon"&gt;
                &lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;
                  &lt;circle cx="12" cy="12" r="3"/&gt;
                  &lt;path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-4 0v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83-2.83l.06-.06A1.65 1.65 0 0 0 4.6 15a1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1 0-4h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 4 0v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/&gt;
                &lt;/svg&gt;
              &lt;/span&gt;
              &lt;span class="tb-label" id="model-btn-label"&gt;Model&lt;/span&gt;
              &lt;span class="tb-caret"&gt;▾&lt;/span&gt;
            &lt;/button&gt;
            &lt;button type="button" class="composer-toolbar-btn" id="mode-btn" title="Switch chat mode"&gt;
              &lt;span class="tb-icon" id="mode-btn-icon"&gt;
                &lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;
                  &lt;path d="M18 13a6 6 0 0 1-6 6 6 6 0 0 1-6-6c0-7 6-11 6-11s6 4 6 11z"/&gt;
                  &lt;circle cx="12" cy="13" r="2"/&gt;
                &lt;/svg&gt;
              &lt;/span&gt;
              &lt;span class="tb-label" id="mode-btn-label"&gt;Ask&lt;/span&gt;
              &lt;span class="tb-caret"&gt;▾&lt;/span&gt;
            &lt;/button&gt;
          &lt;/div&gt;
          &lt;div id="composer-actions-right"&gt;
            &lt;button type="submit" class="send" id="send" title="Send (Enter)"&gt;
              &lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;
                &lt;path d="M12 19V5M5 12l7-7 7 7"/&gt;
              &lt;/svg&gt;
            &lt;/button&gt;
            &lt;button type="button" class="send stop-btn" id="stop" style="display:none;" title="Stop the current turn"&gt;Stop&lt;/button&gt;
          &lt;/div&gt;
        &lt;/div&gt;
      &lt;/form&gt;
    &lt;/div&gt;
  &lt;/section&gt;

  &lt;!-- settings pane --&gt;
  &lt;section id="pane-settings" class="pane"&gt;
    &lt;aside id="settings-sidebar"&gt;
      &lt;div class="sb-item active" data-sg="setup"&gt;Setup&lt;/div&gt;
      &lt;div class="sb-item" data-sg="general"&gt;General&lt;/div&gt;
      &lt;div class="sb-item" data-sg="model"&gt;Model&lt;/div&gt;
      &lt;div class="sb-item" data-sg="providers"&gt;Providers&lt;/div&gt;
      &lt;div class="sb-item" data-sg="rules"&gt;Rules&lt;/div&gt;
      &lt;div class="sb-item" data-sg="sessions"&gt;Session Memory&lt;/div&gt;
      &lt;div class="sb-item" data-sg="usage"&gt;Usage&lt;/div&gt;
      &lt;div class="sb-item" data-sg="permissions"&gt;Permissions&lt;/div&gt;
      &lt;div class="sb-item" data-sg="privacy"&gt;Privacy&lt;/div&gt;
      &lt;div class="sb-item" data-sg="knowledge"&gt;Knowledge&lt;/div&gt;
      &lt;div class="sb-item" data-sg="diagnostics"&gt;Diagnostics&lt;/div&gt;
      &lt;div class="sb-item" data-sg="remote"&gt;📱 Remote&lt;/div&gt;
      &lt;div class="sb-item" data-sg="about"&gt;About&lt;/div&gt;
    &lt;/aside&gt;
    &lt;div id="settings-content"&gt;
      &lt;!-- Setup — first item in sidebar; quick-start guide for all built-in providers --&gt;
      &lt;div class="sg active" data-sg="setup"&gt;
        &lt;h2&gt;Setup — get going in 60 seconds&lt;/h2&gt;
        &lt;div class="meta"&gt;PracSim Modeler AI works with any of the four major AI providers. Pick one, paste your API key (or set the environment variable shown), and chat. You can add more providers later.&lt;/div&gt;

        &lt;h3&gt;Step 1 — Pick a provider&lt;/h3&gt;
        &lt;div class="meta"&gt;All four use the same chat UI, the same tools, the same effort slider. Differences come down to model strengths and pricing. Cheap starting picks for FlexSim work: &lt;strong&gt;Claude Haiku 4.5&lt;/strong&gt; ($1/$5 per Mtok), &lt;strong&gt;GPT-4o-mini&lt;/strong&gt; ($0.15/$0.60), or &lt;strong&gt;Gemini 2.0 Flash&lt;/strong&gt; ($0.10/$0.40). Move up to Sonnet / GPT-4o / Gemini 2.5 Pro when you need more reasoning.&lt;/div&gt;

        &lt;div class="setup-providers"&gt;
          &lt;div class="setup-card"&gt;
            &lt;h4&gt;Anthropic (Claude)&lt;/h4&gt;
            &lt;div class="meta"&gt;Default. Best at structured tool use and code. Models: Haiku 4.5, Sonnet 4.6, Opus 4.7. Get a key at &lt;a href="https://console.anthropic.com/settings/keys" target="_blank" rel="noopener noreferrer"&gt;console.anthropic.com/settings/keys&lt;/a&gt;.&lt;/div&gt;
            &lt;div class="setup-key-options"&gt;
              &lt;div&gt;&lt;strong&gt;In-app:&lt;/strong&gt; Settings → Providers → Anthropic → Set key&lt;/div&gt;
              &lt;div&gt;&lt;strong&gt;Env var:&lt;/strong&gt; &lt;code&gt;ANTHROPIC_API_KEY&lt;/code&gt; (restart FlexSim after setting)&lt;/div&gt;
            &lt;/div&gt;
          &lt;/div&gt;

          &lt;div class="setup-card"&gt;
            &lt;h4&gt;OpenAI (GPT)&lt;/h4&gt;
            &lt;div class="meta"&gt;Broadest model lineup. Models: GPT-4o-mini, GPT-4o, GPT-4.1, o3-mini (thinking), o1 (thinking). Get a key at &lt;a href="https://platform.openai.com/api-keys" target="_blank" rel="noopener noreferrer"&gt;platform.openai.com/api-keys&lt;/a&gt;.&lt;/div&gt;
            &lt;div class="setup-key-options"&gt;
              &lt;div&gt;&lt;strong&gt;In-app:&lt;/strong&gt; Settings → Providers → OpenAI → Set key&lt;/div&gt;
              &lt;div&gt;&lt;strong&gt;Env var:&lt;/strong&gt; &lt;code&gt;OPENAI_API_KEY&lt;/code&gt;&lt;/div&gt;
            &lt;/div&gt;
          &lt;/div&gt;

          &lt;div class="setup-card"&gt;
            &lt;h4&gt;Google Gemini&lt;/h4&gt;
            &lt;div class="meta"&gt;Largest context window (2M tokens on Gemini 2.5 Pro). Models: 2.0 Flash, 2.5 Flash, 2.5 Pro. Get a key at &lt;a href="https://aistudio.google.com/apikey" target="_blank" rel="noopener noreferrer"&gt;aistudio.google.com/apikey&lt;/a&gt;.&lt;/div&gt;
            &lt;div class="setup-key-options"&gt;
              &lt;div&gt;&lt;strong&gt;In-app:&lt;/strong&gt; Settings → Providers → Google Gemini → Set key&lt;/div&gt;
              &lt;div&gt;&lt;strong&gt;Env var:&lt;/strong&gt; &lt;code&gt;GEMINI_API_KEY&lt;/code&gt; (or &lt;code&gt;GOOGLE_API_KEY&lt;/code&gt;)&lt;/div&gt;
            &lt;/div&gt;
          &lt;/div&gt;

          &lt;div class="setup-card"&gt;
            &lt;h4&gt;xAI (Grok)&lt;/h4&gt;
            &lt;div class="meta"&gt;Newer entrant; fast on Grok-3 Mini. Models: Grok 3 Mini, Grok 3, Grok 2 Vision. Get a key at &lt;a href="https://console.x.ai/" target="_blank" rel="noopener noreferrer"&gt;console.x.ai&lt;/a&gt;.&lt;/div&gt;
            &lt;div class="setup-key-options"&gt;
              &lt;div&gt;&lt;strong&gt;In-app:&lt;/strong&gt; Settings → Providers → xAI → Set key&lt;/div&gt;
              &lt;div&gt;&lt;strong&gt;Env var:&lt;/strong&gt; &lt;code&gt;XAI_API_KEY&lt;/code&gt;&lt;/div&gt;
            &lt;/div&gt;
          &lt;/div&gt;

          &lt;div class="setup-card"&gt;
            &lt;h4&gt;Mistral&lt;/h4&gt;
            &lt;div class="meta"&gt;French; strong on code and instruction-following. Models: Mistral Small, Medium, Large, plus Codestral (code-tuned, 256K context). Get a key at &lt;a href="https://console.mistral.ai/api-keys/" target="_blank" rel="noopener noreferrer"&gt;console.mistral.ai/api-keys&lt;/a&gt;.&lt;/div&gt;
            &lt;div class="setup-key-options"&gt;
              &lt;div&gt;&lt;strong&gt;In-app:&lt;/strong&gt; Settings → Providers → Mistral → Set key&lt;/div&gt;
              &lt;div&gt;&lt;strong&gt;Env var:&lt;/strong&gt; &lt;code&gt;MISTRAL_API_KEY&lt;/code&gt;&lt;/div&gt;
            &lt;/div&gt;
          &lt;/div&gt;
        &lt;/div&gt;

        &lt;h3&gt;Step 2 — How keys are stored&lt;/h3&gt;
        &lt;div class="meta"&gt;Pasting a key in-app encrypts it via Windows DPAPI at &lt;code&gt;%APPDATA%/PRACSIM/ModelerAI/keys.dat&lt;/code&gt; — decryptable only by your Windows user on this machine, never written plaintext to disk, never sent across the bridge to JavaScript, never visible to the AI model. The env-var fallback is for IT teams who want to provision keys centrally (Group Policy, system env vars) without users typing.&lt;/div&gt;

        &lt;h3&gt;Step 3 — Pick a model and chat&lt;/h3&gt;
        &lt;div class="meta"&gt;Settings → Model shows every model from every provider you've configured a key for, in one flat list. Switch any time; the next message uses the new model.&lt;/div&gt;

        &lt;h3&gt;Step 4 — (Optional) Add more providers&lt;/h3&gt;
        &lt;div class="meta"&gt;Don't see your provider above? The "Add custom provider" form at the bottom of Settings → Providers accepts any OpenAI-Chat-Completions-compatible endpoint. Works with DeepSeek, Mistral, Groq, Together, Fireworks, OpenRouter, Ollama (local), LM Studio (local), llama.cpp server, LiteLLM, and most others.&lt;/div&gt;

        &lt;h3&gt;Security and privacy&lt;/h3&gt;
        &lt;div class="meta"&gt;PracSim Solutions has no servers in the data path — your prompts, model contents, and AI responses go directly from your computer to whichever provider you selected. We never see them. The DPAPI-encrypted keystore + env-var fallback combination means even the LLM running inside Modeler AI cannot read your raw API keys. See Settings → Privacy for each provider's trust posture.&lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- General --&gt;
      &lt;div class="sg" data-sg="general"&gt;
        &lt;h2&gt;General&lt;/h2&gt;
        &lt;h3&gt;Theme&lt;/h3&gt;
        &lt;div class="meta"&gt;Switch the panel's color scheme. "System" follows your Windows light/dark preference if FlexSim exposes it; falls back to Dark otherwise.&lt;/div&gt;
        &lt;div class="radio-group" id="theme-group"&gt;
          &lt;label&gt;&lt;input type="radio" name="theme" value="light"&gt; Light&lt;/label&gt;
          &lt;label&gt;&lt;input type="radio" name="theme" value="dark"&gt; Dark&lt;/label&gt;
          &lt;label&gt;&lt;input type="radio" name="theme" value="system"&gt; System&lt;/label&gt;
        &lt;/div&gt;
        &lt;h3&gt;Input&lt;/h3&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Send on Enter&lt;/div&gt;
            &lt;div class="desc"&gt;When on, Enter sends; Shift+Enter inserts newline. When off, Ctrl+Enter sends.&lt;/div&gt;
          &lt;/div&gt;
          &lt;label&gt;&lt;input type="checkbox" id="send-on-enter" checked&gt; &lt;/label&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Show debug info on assistant turns&lt;/div&gt;
            &lt;div class="desc"&gt;Adds a small token-count footer to each assistant reply.&lt;/div&gt;
          &lt;/div&gt;
          &lt;label&gt;&lt;input type="checkbox" id="show-debug"&gt; &lt;/label&gt;
        &lt;/div&gt;
        &lt;h3&gt;Export&lt;/h3&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Export folder&lt;/div&gt;
            &lt;div class="desc"&gt;Where &lt;code&gt;/export&lt;/code&gt; writes conversation markdown files. Empty falls back to &lt;code&gt;%APPDATA%\PRACSIM\ModelerAI\exports\&lt;/code&gt;.&lt;/div&gt;
          &lt;/div&gt;
          &lt;input type="text" id="export-dir" placeholder="C:\Users\you\Documents\ModelerAI exports\" style="flex:1; max-width:380px;"&gt;
        &lt;/div&gt;
        &lt;h3&gt;Reset&lt;/h3&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Reset user preferences&lt;/div&gt;
            &lt;div class="desc"&gt;Restores theme / mode / cost ceiling / export folder and other preferences to defaults. Does NOT clear API keys, terms acceptance, custom providers, or the onboarding-completed flag.&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="reset-defaults" class="reset-btn"&gt;Reset to defaults…&lt;/button&gt;
        &lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Model --&gt;
      &lt;div class="sg" data-sg="model"&gt;
        &lt;h2&gt;Model&lt;/h2&gt;
        &lt;div class="meta"&gt;Pick the model that handles chat — every model from every configured provider appears here in one list. Switch any time; subsequent turns use the new model. Provider keys are configured in Settings → Providers.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Active model&lt;/div&gt;
            &lt;div class="desc" id="model-pricing"&gt;Loading pricing...&lt;/div&gt;
          &lt;/div&gt;
          &lt;select id="model-select"&gt;&lt;option&gt;Loading...&lt;/option&gt;&lt;/select&gt;
        &lt;/div&gt;
        &lt;h3&gt;Capabilities&lt;/h3&gt;
        &lt;div class="pill-list" id="model-caps"&gt;
          &lt;span class="badge badge-grey"&gt;Loading...&lt;/span&gt;
        &lt;/div&gt;

        &lt;h3&gt;Effort (extended thinking)&lt;/h3&gt;
        &lt;div class="meta"&gt;Higher effort = more silent reasoning per turn = better answers on hard questions, at extra output-token cost. Off is cheapest. Only models with thinking support honor this setting.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Effort level&lt;/div&gt;
          &lt;/div&gt;
          &lt;select id="effort-select"&gt;
            &lt;option value="off"&gt;Off (cheapest)&lt;/option&gt;
            &lt;option value="low"&gt;Low (~2k tokens)&lt;/option&gt;
            &lt;option value="medium"&gt;Medium (~8k tokens)&lt;/option&gt;
            &lt;option value="high"&gt;High (~16k tokens)&lt;/option&gt;
          &lt;/select&gt;
        &lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Providers --&gt;
      &lt;div class="sg" data-sg="providers"&gt;
        &lt;h2&gt;Providers&lt;/h2&gt;
        &lt;div class="meta"&gt;Four mainstream providers ship in-box. Pick one (or several) and paste an API key — keys are DPAPI-encrypted at rest. Each provider also supports an environment-variable fallback for IT/enterprise provisioning. Don't see what you want? Add any OpenAI-Chat-Completions-compatible endpoint as a custom provider below (Ollama, LM Studio, OpenAI-API-clones, etc.).&lt;/div&gt;

        &lt;h3&gt;Built-in providers&lt;/h3&gt;

        &lt;!-- Anthropic --&gt;
        &lt;div class="row" id="row-prov-anthropic"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Anthropic (Claude)&lt;/div&gt;
            &lt;div class="desc" id="anth-status-msg"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;span class="badge badge-grey" id="anth-badge"&gt;Loading…&lt;/span&gt;
          &lt;button id="anth-test" title="Validate the key with a 1-token completion (~$0.0001)."&gt;Test key&lt;/button&gt;
          &lt;button id="anth-set" title="Paste your sk-ant-… key. Stored DPAPI-encrypted; never written plaintext to disk."&gt;Set key&lt;/button&gt;
          &lt;button id="anth-forget" class="danger" title="Remove the key from the keystore. If ANTHROPIC_API_KEY is set in your environment, that fallback continues to work."&gt;Forget&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row" id="row-anth-keyform" style="display:none;"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Paste Anthropic key&lt;/div&gt;
            &lt;div class="desc"&gt;Stored DPAPI-encrypted under &lt;code&gt;%APPDATA%/PRACSIM/ModelerAI/keys.dat&lt;/code&gt;. Env-var fallback: &lt;code&gt;ANTHROPIC_API_KEY&lt;/code&gt;.&lt;/div&gt;
          &lt;/div&gt;
          &lt;input id="anth-key-input" type="password" placeholder="sk-ant-…" style="min-width:280px;"&gt;
          &lt;button id="anth-key-save" class="primary"&gt;Save&lt;/button&gt;
          &lt;button id="anth-key-cancel"&gt;Cancel&lt;/button&gt;
        &lt;/div&gt;

        &lt;!-- OpenAI --&gt;
        &lt;div class="row" id="row-prov-openai"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;OpenAI (GPT)&lt;/div&gt;
            &lt;div class="desc" id="openai-status-msg"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;span class="badge badge-grey" id="openai-badge"&gt;Loading…&lt;/span&gt;
          &lt;button id="openai-test" title="Validate the key with a 1-token completion against gpt-4o-mini."&gt;Test key&lt;/button&gt;
          &lt;button id="openai-set" title="Paste your sk-… OpenAI key. Stored DPAPI-encrypted."&gt;Set key&lt;/button&gt;
          &lt;button id="openai-forget" class="danger" title="Remove the key from the keystore. If OPENAI_API_KEY is set in your environment, that fallback continues to work."&gt;Forget&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row" id="row-openai-keyform" style="display:none;"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Paste OpenAI key&lt;/div&gt;
            &lt;div class="desc"&gt;Get one at &lt;a href="https://platform.openai.com/api-keys" target="_blank" rel="noopener noreferrer"&gt;platform.openai.com/api-keys&lt;/a&gt;. Env-var fallback: &lt;code&gt;OPENAI_API_KEY&lt;/code&gt;.&lt;/div&gt;
          &lt;/div&gt;
          &lt;input id="openai-key-input" type="password" placeholder="sk-…" style="min-width:280px;"&gt;
          &lt;button id="openai-key-save" class="primary"&gt;Save&lt;/button&gt;
          &lt;button id="openai-key-cancel"&gt;Cancel&lt;/button&gt;
        &lt;/div&gt;

        &lt;!-- Gemini --&gt;
        &lt;div class="row" id="row-prov-gemini"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Google Gemini&lt;/div&gt;
            &lt;div class="desc" id="gemini-status-msg"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;span class="badge badge-grey" id="gemini-badge"&gt;Loading…&lt;/span&gt;
          &lt;button id="gemini-test" title="Validate the key with a 1-token completion against gemini-2.0-flash."&gt;Test key&lt;/button&gt;
          &lt;button id="gemini-set" title="Paste your Google AI Studio API key. Stored DPAPI-encrypted."&gt;Set key&lt;/button&gt;
          &lt;button id="gemini-forget" class="danger" title="Remove the key from the keystore. If GEMINI_API_KEY (or GOOGLE_API_KEY) is set in your environment, that fallback continues to work."&gt;Forget&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row" id="row-gemini-keyform" style="display:none;"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Paste Gemini key&lt;/div&gt;
            &lt;div class="desc"&gt;Get one at &lt;a href="https://aistudio.google.com/apikey" target="_blank" rel="noopener noreferrer"&gt;aistudio.google.com/apikey&lt;/a&gt;. Env-var fallback: &lt;code&gt;GEMINI_API_KEY&lt;/code&gt; or &lt;code&gt;GOOGLE_API_KEY&lt;/code&gt;.&lt;/div&gt;
          &lt;/div&gt;
          &lt;input id="gemini-key-input" type="password" placeholder="AIza…" style="min-width:280px;"&gt;
          &lt;button id="gemini-key-save" class="primary"&gt;Save&lt;/button&gt;
          &lt;button id="gemini-key-cancel"&gt;Cancel&lt;/button&gt;
        &lt;/div&gt;

        &lt;!-- Mistral --&gt;
        &lt;div class="row" id="row-prov-mistral"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Mistral&lt;/div&gt;
            &lt;div class="desc" id="mistral-status-msg"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;span class="badge badge-grey" id="mistral-badge"&gt;Loading…&lt;/span&gt;
          &lt;button id="mistral-test" title="Validate the key with a 1-token completion against mistral-small-latest."&gt;Test key&lt;/button&gt;
          &lt;button id="mistral-set" title="Paste your Mistral API key. Stored DPAPI-encrypted."&gt;Set key&lt;/button&gt;
          &lt;button id="mistral-forget" class="danger" title="Remove the key from the keystore. If MISTRAL_API_KEY is set in your environment, that fallback continues to work."&gt;Forget&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row" id="row-mistral-keyform" style="display:none;"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Paste Mistral key&lt;/div&gt;
            &lt;div class="desc"&gt;Get one at &lt;a href="https://console.mistral.ai/api-keys/" target="_blank" rel="noopener noreferrer"&gt;console.mistral.ai/api-keys&lt;/a&gt;. Env-var fallback: &lt;code&gt;MISTRAL_API_KEY&lt;/code&gt;.&lt;/div&gt;
          &lt;/div&gt;
          &lt;input id="mistral-key-input" type="password" placeholder="…" style="min-width:280px;"&gt;
          &lt;button id="mistral-key-save" class="primary"&gt;Save&lt;/button&gt;
          &lt;button id="mistral-key-cancel"&gt;Cancel&lt;/button&gt;
        &lt;/div&gt;

        &lt;!-- xAI Grok --&gt;
        &lt;div class="row" id="row-prov-xai"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;xAI (Grok)&lt;/div&gt;
            &lt;div class="desc" id="xai-status-msg"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;span class="badge badge-grey" id="xai-badge"&gt;Loading…&lt;/span&gt;
          &lt;button id="xai-test" title="Validate the key with a 1-token completion against grok-3-mini."&gt;Test key&lt;/button&gt;
          &lt;button id="xai-set" title="Paste your xAI API key. Stored DPAPI-encrypted."&gt;Set key&lt;/button&gt;
          &lt;button id="xai-forget" class="danger" title="Remove the key from the keystore. If XAI_API_KEY is set in your environment, that fallback continues to work."&gt;Forget&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row" id="row-xai-keyform" style="display:none;"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Paste xAI key&lt;/div&gt;
            &lt;div class="desc"&gt;Get one at &lt;a href="https://console.x.ai/" target="_blank" rel="noopener noreferrer"&gt;console.x.ai&lt;/a&gt;. Env-var fallback: &lt;code&gt;XAI_API_KEY&lt;/code&gt;.&lt;/div&gt;
          &lt;/div&gt;
          &lt;input id="xai-key-input" type="password" placeholder="xai-…" style="min-width:280px;"&gt;
          &lt;button id="xai-key-save" class="primary"&gt;Save&lt;/button&gt;
          &lt;button id="xai-key-cancel"&gt;Cancel&lt;/button&gt;
        &lt;/div&gt;

        &lt;h3&gt;Custom providers&lt;/h3&gt;
        &lt;div id="custom-providers-list"&gt;
          &lt;div class="meta"&gt;No custom providers configured. Use the form below to add one.&lt;/div&gt;
        &lt;/div&gt;

        &lt;h3&gt;Add a custom provider&lt;/h3&gt;
        &lt;div class="meta"&gt;All fields except pricing are required. For local providers like Ollama / LM Studio, the API key can be any non-empty string (most local servers ignore it).&lt;/div&gt;
        &lt;div class="form-grid"&gt;
          &lt;label&gt;Display name&lt;input id="cp-display-name" type="text" placeholder="e.g. Local Ollama (Llama 3.2)"&gt;&lt;/label&gt;
          &lt;label&gt;Base URL&lt;input id="cp-base-url" type="text" placeholder="http://localhost:11434/v1"&gt;&lt;/label&gt;
          &lt;label&gt;API key&lt;input id="cp-api-key" type="text" placeholder="sk-… or 'ollama' for local"&gt;&lt;/label&gt;
          &lt;label&gt;Model ID&lt;input id="cp-model-id" type="text" placeholder="llama3.2:3b"&gt;&lt;/label&gt;
          &lt;label&gt;Model display name (optional)&lt;input id="cp-model-display" type="text" placeholder="defaults to Model ID"&gt;&lt;/label&gt;
          &lt;label&gt;Input $/Mtok (optional)&lt;input id="cp-input-cost" type="number" step="0.01" placeholder="0.00"&gt;&lt;/label&gt;
          &lt;label&gt;Output $/Mtok (optional)&lt;input id="cp-output-cost" type="number" step="0.01" placeholder="0.00"&gt;&lt;/label&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="desc"&gt;For Ollama: base URL &lt;code&gt;http://localhost:11434/v1&lt;/code&gt; · API key any string · model ID matches whatever you ran &lt;code&gt;ollama pull&lt;/code&gt; on (e.g. &lt;code&gt;llama3.2:3b&lt;/code&gt;).&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="cp-add" class="primary"&gt;Add provider&lt;/button&gt;
        &lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Rules --&gt;
      &lt;div class="sg" data-sg="rules"&gt;
        &lt;h2&gt;Rules&lt;/h2&gt;
        &lt;div class="meta"&gt;User rules are markdown instructions injected into every system prompt. Edit the file in your default editor; changes take effect on the next message.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;My rules file&lt;/div&gt;
            &lt;div class="desc" id="rules-path"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="rules-open" title="Copy the file path to your clipboard so you can paste it into File Explorer or your editor."&gt;Copy path&lt;/button&gt;
          &lt;button id="rules-refresh" title="Reload the file from disk so the preview below reflects your latest edits."&gt;Refresh&lt;/button&gt;
        &lt;/div&gt;
        &lt;h3&gt;Preview (first ~10 lines)&lt;/h3&gt;
        &lt;div class="preview-box" id="rules-preview"&gt;Loading...&lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Session Memory --&gt;
      &lt;div class="sg" data-sg="sessions"&gt;
        &lt;h2&gt;Session Memory&lt;/h2&gt;
        &lt;div class="meta"&gt;Each conversation is anchored to the in-model ModelerAI object — the GUID lives in the model tree, so renaming or reopening the &lt;code&gt;.fsm&lt;/code&gt; picks the chat back up automatically. Files are DPAPI-encrypted under &lt;code&gt;%APPDATA%/PRACSIM/ModelerAI/sessions/&lt;/code&gt;. Conversations opened without a model anchor stay in-memory only (the yellow ⚓ bar at the top of the chat shows this state).&lt;/div&gt;

        &lt;h3&gt;Session scope&lt;/h3&gt;
        &lt;div class="meta"&gt;Most users should leave this on &lt;strong&gt;Per model&lt;/strong&gt;. &lt;strong&gt;Per folder&lt;/strong&gt; is for shared-history workflows; &lt;strong&gt;Manual&lt;/strong&gt; is for explicitly linking to a specific past session.&lt;/div&gt;
        &lt;div class="radio-group stacked" id="session-strategy-group"&gt;
          &lt;label&gt;
            &lt;input type="radio" name="session_strategy" value="per-file"&gt;
            &lt;span&gt;&lt;strong&gt;Per model&lt;/strong&gt; &lt;span class="meta-inline"&gt;— anchored to the in-model ModelerAI object's GUID&lt;/span&gt;&lt;/span&gt;
          &lt;/label&gt;
          &lt;label&gt;
            &lt;input type="radio" name="session_strategy" value="per-folder"&gt;
            &lt;span&gt;&lt;strong&gt;Per folder&lt;/strong&gt; &lt;span class="meta-inline"&gt;— every &lt;code&gt;.fsm&lt;/code&gt; in the same directory shares one session&lt;/span&gt;&lt;/span&gt;
          &lt;/label&gt;
          &lt;label&gt;
            &lt;input type="radio" name="session_strategy" value="manual"&gt;
            &lt;span&gt;&lt;strong&gt;Manual&lt;/strong&gt; &lt;span class="meta-inline"&gt;— pick a specific saved session from the list below&lt;/span&gt;&lt;/span&gt;
          &lt;/label&gt;
        &lt;/div&gt;

        &lt;div id="manual-picker" style="display:none; margin-top:10px;"&gt;
          &lt;div class="row"&gt;
            &lt;div class="label"&gt;&lt;div class="name"&gt;Linked session&lt;/div&gt;&lt;div class="desc"&gt;Conversation continues from whichever session you pick.&lt;/div&gt;&lt;/div&gt;
            &lt;select id="manual-session-select"&gt;&lt;option value=""&gt;(none — uses per-model)&lt;/option&gt;&lt;/select&gt;
          &lt;/div&gt;
        &lt;/div&gt;

        &lt;h3&gt;Save-As behavior&lt;/h3&gt;
        &lt;div class="meta"&gt;What happens to the chat when you Save-As a model to a new folder. Detected automatically the next time the panel polls FlexSim (typically within a second). &lt;strong&gt;Same-folder Save-As with just a filename change isn't auto-detected&lt;/strong&gt; — the FlexSim SDK doesn't expose the current file's name, only its directory.&lt;/div&gt;
        &lt;div class="radio-group stacked" id="save-as-behavior-group"&gt;
          &lt;label&gt;
            &lt;input type="radio" name="save_as_behavior" value="copy"&gt;
            &lt;span&gt;&lt;strong&gt;Copy conversation&lt;/strong&gt; &lt;span class="meta-inline"&gt;— new model gets a fresh GUID + duplicate of the chat; the two diverge from here (DEFAULT, matches the scenario-branching workflow)&lt;/span&gt;&lt;/span&gt;
          &lt;/label&gt;
          &lt;label&gt;
            &lt;input type="radio" name="save_as_behavior" value="fresh"&gt;
            &lt;span&gt;&lt;strong&gt;Fresh start&lt;/strong&gt; &lt;span class="meta-inline"&gt;— new model gets a fresh GUID and an empty chat; original model keeps its chat untouched&lt;/span&gt;&lt;/span&gt;
          &lt;/label&gt;
          &lt;label&gt;
            &lt;input type="radio" name="save_as_behavior" value="share"&gt;
            &lt;span&gt;&lt;strong&gt;Share (legacy)&lt;/strong&gt; &lt;span class="meta-inline"&gt;— both files keep writing to the same on-disk session; chatting in either bleeds into the other&lt;/span&gt;&lt;/span&gt;
          &lt;/label&gt;
        &lt;/div&gt;

        &lt;h3&gt;Current conversation&lt;/h3&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Anchor&lt;/div&gt;
            &lt;div class="desc" id="session-key"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;span id="session-count" class="badge badge-grey"&gt;0 messages&lt;/span&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;File&lt;/div&gt;
            &lt;div class="desc" id="session-file"&gt;Loading…&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="session-open-folder" title="Copy the sessions folder path to your clipboard."&gt;Copy folder path&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="desc"&gt;Wipes the in-memory chat AND the on-disk file for the current anchor. Doesn't touch other sessions in the list below.&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="session-clear" title="Wipe the in-memory chat AND delete the on-disk session file for the current anchor." class="danger"&gt;Clear current conversation&lt;/button&gt;
        &lt;/div&gt;

        &lt;h3&gt;Export&lt;/h3&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Export conversation as Markdown&lt;/div&gt;
            &lt;div class="desc"&gt;Writes a single &lt;code&gt;.md&lt;/code&gt; file with every user prompt, assistant reply, tool call (args + result), and reasoning. Lands under &lt;code&gt;%APPDATA%/PRACSIM/ModelerAI/exports/&lt;/code&gt;. Paste the path into wherever you want to diagnose the AI's behavior.&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="session-export" class="primary"&gt;Export to Markdown&lt;/button&gt;
        &lt;/div&gt;
        &lt;div id="export-status" class="meta" style="display:none;"&gt;&lt;/div&gt;

        &lt;h3&gt;Saved sessions&lt;/h3&gt;
        &lt;div class="meta"&gt;All conversations ModelerAI has saved on this machine for your Windows user. Files from other users or other machines stay encrypted and unreadable but still appear here so you can clean them up.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;&lt;div class="desc" id="sm-summary"&gt;Loading saved sessions…&lt;/div&gt;&lt;/div&gt;
          &lt;button id="sm-refresh" title="Re-scan the sessions folder."&gt;Refresh&lt;/button&gt;
          &lt;button id="sm-delete-all" class="danger" title="Delete every saved conversation file. Cannot be undone."&gt;Delete all&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="sm-list" id="sm-list"&gt;&lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Usage --&gt;
      &lt;div class="sg" data-sg="usage"&gt;
        &lt;h2&gt;Usage&lt;/h2&gt;
        &lt;div class="meta"&gt;Token counters for this session. Cleared by &lt;code&gt;/clear&lt;/code&gt; or the Reset button below. Cost estimate uses Sonnet 4.6 rates.&lt;/div&gt;
        &lt;div class="stat-grid"&gt;
          &lt;div class="stat"&gt;&lt;div class="val" id="stat-input"&gt;0&lt;/div&gt;&lt;div class="lbl"&gt;Input tokens&lt;/div&gt;&lt;/div&gt;
          &lt;div class="stat"&gt;&lt;div class="val" id="stat-output"&gt;0&lt;/div&gt;&lt;div class="lbl"&gt;Output tokens&lt;/div&gt;&lt;/div&gt;
          &lt;div class="stat"&gt;&lt;div class="val" id="stat-cache-read"&gt;0&lt;/div&gt;&lt;div class="lbl"&gt;Cache-read tokens&lt;/div&gt;&lt;/div&gt;
          &lt;div class="stat"&gt;&lt;div class="val" id="stat-turns"&gt;0&lt;/div&gt;&lt;div class="lbl"&gt;Turns&lt;/div&gt;&lt;/div&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Estimated cost (Sonnet 4.6)&lt;/div&gt;
            &lt;div class="desc"&gt;$3/Mtok input, $15/Mtok output, $0.30/Mtok cache-read, $3.75/Mtok cache-write.&lt;/div&gt;
          &lt;/div&gt;
          &lt;span id="stat-cost" style="font-weight:700;"&gt;$0.0000&lt;/span&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;&lt;div class="desc"&gt;Resetting only zeroes the counters - doesn't affect the conversation. Use &lt;code&gt;/clear&lt;/code&gt; to do both.&lt;/div&gt;&lt;/div&gt;
          &lt;button id="usage-reset"&gt;Reset counters&lt;/button&gt;
        &lt;/div&gt;

        &lt;h3&gt;Cost ceiling&lt;/h3&gt;
        &lt;div class="meta"&gt;Soft warn + hard stop on cumulative session cost. Zero means no limit. Stats reset on &lt;code&gt;/clear&lt;/code&gt;, so the ceiling is per-conversation, not per-month.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;&lt;div class="name"&gt;Warn at (USD)&lt;/div&gt;&lt;div class="desc"&gt;Inline banner above the composer once session cost passes this; turn still proceeds.&lt;/div&gt;&lt;/div&gt;
          &lt;input type="number" id="cost-warn" step="0.05" min="0" placeholder="0.50" style="width:120px;"&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;&lt;div class="name"&gt;Hard stop at (USD)&lt;/div&gt;&lt;div class="desc"&gt;Refuses to send further turns once session cost passes this. Use to prevent runaway costs.&lt;/div&gt;&lt;/div&gt;
          &lt;input type="number" id="cost-stop" step="0.5" min="0" placeholder="5.00" style="width:120px;"&gt;
        &lt;/div&gt;

        &lt;h3&gt;Agent loop cap&lt;/h3&gt;
        &lt;div class="meta"&gt;Maximum number of LLM ↔ tool ping-pong rounds within a single turn. Each round is one Anthropic API call where the model emits tool calls, the runtime runs them, and the model continues. Hitting the cap aborts the turn with an error. Default 25; raise for complex multi-step builds, lower as a tighter safety brake.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;&lt;div class="name"&gt;Max tool iterations&lt;/div&gt;&lt;div class="desc"&gt;Range 1–500. Real production tool flows usually settle in 3–15 iterations; 25 is a comfortable default.&lt;/div&gt;&lt;/div&gt;
          &lt;input type="number" id="max-tool-iterations" step="1" min="1" max="500" placeholder="25" style="width:120px;"&gt;
        &lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Permissions --&gt;
      &lt;div class="sg" data-sg="permissions"&gt;
        &lt;h2&gt;Permissions&lt;/h2&gt;
        &lt;div class="placeholder-block"&gt;Permission settings will land in v0.2 with the tool framework. You'll be able to: choose per-tool auto-approve / ask-each-time / always-deny; set Plan / Ask / Apply mode defaults; gate filesystem and lifecycle actions individually.&lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Privacy --&gt;
      &lt;div class="sg" data-sg="privacy"&gt;
        &lt;h2&gt;Privacy&lt;/h2&gt;
        &lt;div class="meta"&gt;PracSim ModelerAI uses BYOK (bring-your-own-key) and posts directly from your computer to the AI provider you select. PracSim Solutions has no servers in this data path and never sees the content of your prompts, model data, or AI responses. The table below summarizes each provider's published policy at the time of this release. &lt;strong&gt;Provider policies change — always verify the linked source before sending sensitive data.&lt;/strong&gt;&lt;/div&gt;

        &lt;h3&gt;Provider trust posture&lt;/h3&gt;
        &lt;table class="priv-table"&gt;
          &lt;thead&gt;
            &lt;tr&gt;&lt;th&gt;Provider&lt;/th&gt;&lt;th&gt;Trains on API data?&lt;/th&gt;&lt;th&gt;Retention&lt;/th&gt;&lt;th&gt;ZDR&lt;/th&gt;&lt;th&gt;Verdict&lt;/th&gt;&lt;th&gt;Source&lt;/th&gt;&lt;/tr&gt;
          &lt;/thead&gt;
          &lt;tbody&gt;
            &lt;tr class="priv-row priv-green"&gt;
              &lt;td&gt;&lt;strong&gt;Anthropic Claude API&lt;/strong&gt;&lt;br&gt;&lt;span class="meta-inline"&gt;api.anthropic.com&lt;/span&gt;&lt;/td&gt;
              &lt;td&gt;No (Developer Partner Program opt-IN only)&lt;/td&gt;
              &lt;td&gt;7 days (auto-deleted)&lt;/td&gt;
              &lt;td&gt;Sales-gated enterprise contract&lt;/td&gt;
              &lt;td&gt;✅ Default — safe&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://www.anthropic.com/legal/commercial-terms" target="_blank" rel="noopener noreferrer"&gt;Commercial Terms&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-green"&gt;
              &lt;td&gt;&lt;strong&gt;OpenAI API&lt;/strong&gt;&lt;br&gt;&lt;span class="meta-inline"&gt;api.openai.com&lt;/span&gt;&lt;/td&gt;
              &lt;td&gt;No (since March 2023)&lt;/td&gt;
              &lt;td&gt;30 days for abuse monitoring&lt;/td&gt;
              &lt;td&gt;Sales-gated&lt;/td&gt;
              &lt;td&gt;✅ Safe&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://openai.com/enterprise-privacy/" target="_blank" rel="noopener noreferrer"&gt;Enterprise Privacy&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-green"&gt;
              &lt;td&gt;&lt;strong&gt;Groq&lt;/strong&gt;&lt;/td&gt;
              &lt;td&gt;No (account toggle)&lt;/td&gt;
              &lt;td&gt;&amp;lt;30 days abuse logs only&lt;/td&gt;
              &lt;td&gt;Self-serve toggle&lt;/td&gt;
              &lt;td&gt;✅ Safe&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://console.groq.com/docs/your-data" target="_blank" rel="noopener noreferrer"&gt;Your Data&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-green"&gt;
              &lt;td&gt;&lt;strong&gt;Mistral&lt;/strong&gt; &lt;span class="meta-inline"&gt;(EU-hosted)&lt;/span&gt;&lt;/td&gt;
              &lt;td&gt;No by default; opt-out available&lt;/td&gt;
              &lt;td&gt;30 days rolling&lt;/td&gt;
              &lt;td&gt;Self-serve&lt;/td&gt;
              &lt;td&gt;✅ Safe&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://legal.mistral.ai/terms/privacy-policy" target="_blank" rel="noopener noreferrer"&gt;Privacy Policy&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-green"&gt;
              &lt;td&gt;&lt;strong&gt;Together.ai / Fireworks.ai&lt;/strong&gt;&lt;/td&gt;
              &lt;td&gt;No without opt-in&lt;/td&gt;
              &lt;td&gt;Configurable&lt;/td&gt;
              &lt;td&gt;Yes&lt;/td&gt;
              &lt;td&gt;✅ Safe&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://www.together.ai/privacy" target="_blank" rel="noopener noreferrer"&gt;Together&lt;/a&gt; · &lt;a href="https://fireworks.ai/privacy-policy" target="_blank" rel="noopener noreferrer"&gt;Fireworks&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-amber"&gt;
              &lt;td&gt;&lt;strong&gt;OpenRouter&lt;/strong&gt;&lt;/td&gt;
              &lt;td&gt;Per-route (varies upstream)&lt;/td&gt;
              &lt;td&gt;Per-route&lt;/td&gt;
              &lt;td&gt;Per-request; account-wide toggle&lt;/td&gt;
              &lt;td&gt;⚠️ Enable ZDR-only routing in their dashboard&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://openrouter.ai/docs/guides/features/zdr" target="_blank" rel="noopener noreferrer"&gt;ZDR docs&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-amber"&gt;
              &lt;td&gt;&lt;strong&gt;xAI Grok API&lt;/strong&gt;&lt;/td&gt;
              &lt;td&gt;&lt;strong&gt;Ambiguous&lt;/strong&gt; — no explicit "API doesn't train" line&lt;/td&gt;
              &lt;td&gt;Not clearly stated&lt;/td&gt;
              &lt;td&gt;Not advertised&lt;/td&gt;
              &lt;td&gt;⚠️ Treat as risky until xAI clarifies&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://x.ai/legal/privacy-policy" target="_blank" rel="noopener noreferrer"&gt;Privacy Policy&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-red"&gt;
              &lt;td&gt;&lt;strong&gt;DeepSeek&lt;/strong&gt;&lt;/td&gt;
              &lt;td&gt;&lt;strong&gt;YES — explicitly trains on customer data&lt;/strong&gt;&lt;/td&gt;
              &lt;td&gt;Indefinite ("as long as account exists")&lt;/td&gt;
              &lt;td&gt;None advertised&lt;/td&gt;
              &lt;td&gt;🚫 &lt;strong&gt;Do not use for sensitive data.&lt;/strong&gt; China jurisdiction; CNIL/Irish DPC probes 2025; prior ClickHouse breach exposed API keys + chat logs&lt;/td&gt;
              &lt;td&gt;&lt;a href="https://cdn.deepseek.com/policies/en-US/deepseek-privacy-policy.html" target="_blank" rel="noopener noreferrer"&gt;Privacy Policy&lt;/a&gt;&lt;/td&gt;
            &lt;/tr&gt;
            &lt;tr class="priv-row priv-green"&gt;
              &lt;td&gt;&lt;strong&gt;Local runners&lt;/strong&gt;&lt;br&gt;&lt;span class="meta-inline"&gt;Ollama, LM Studio, llama.cpp&lt;/span&gt;&lt;/td&gt;
              &lt;td&gt;N/A — runs on your machine&lt;/td&gt;
              &lt;td&gt;N/A&lt;/td&gt;
              &lt;td&gt;N/A&lt;/td&gt;
              &lt;td&gt;✅ Maximum confidentiality&lt;/td&gt;
              &lt;td&gt;—&lt;/td&gt;
            &lt;/tr&gt;
          &lt;/tbody&gt;
        &lt;/table&gt;

        &lt;h3&gt;What PracSim ModelerAI does&lt;/h3&gt;
        &lt;ul class="priv-bullets"&gt;
          &lt;li&gt;&lt;strong&gt;No data collection by PracSim.&lt;/strong&gt; Your prompts, model contents, and AI responses pass directly from your computer to the provider you selected. PracSim Solutions operates no server in this data path.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;BYOK with DPAPI-encrypted at-rest storage.&lt;/strong&gt; Your API keys are encrypted using Windows DPAPI tied to your user account. Plaintext keys never reach the viewer after you type them.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;Encrypted local session files.&lt;/strong&gt; Conversation history under &lt;code&gt;%APPDATA%\PRACSIM\ModelerAI\sessions\&lt;/code&gt; is also DPAPI-encrypted — readable only by your Windows user on this machine.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;No telemetry.&lt;/strong&gt; ModelerAI never phones home, never reports usage, never tracks prompts.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;Custom providers are on you.&lt;/strong&gt; Adding a third-party provider requires explicit acknowledgement of their terms (logged with timestamp in your global settings).&lt;/li&gt;
        &lt;/ul&gt;

        &lt;h3&gt;For top-tier confidentiality: Zero Data Retention (ZDR)&lt;/h3&gt;
        &lt;div class="meta"&gt;Anthropic and OpenAI both offer enterprise contracts that drop retention to zero — useful for HIPAA / ITAR / classified work. &lt;strong&gt;This is a paid arrangement directly between you and the provider&lt;/strong&gt;, not something PracSim can enable. To request ZDR:&lt;/div&gt;
        &lt;ul class="priv-bullets"&gt;
          &lt;li&gt;&lt;strong&gt;Anthropic:&lt;/strong&gt; Contact &lt;a href="https://www.anthropic.com/contact-sales" target="_blank" rel="noopener noreferrer"&gt;Anthropic sales&lt;/a&gt; and ask for a ZDR addendum to your Commercial Agreement.&lt;/li&gt;
          &lt;li&gt;&lt;strong&gt;OpenAI:&lt;/strong&gt; Contact &lt;a href="https://openai.com/contact-sales/" target="_blank" rel="noopener noreferrer"&gt;OpenAI sales&lt;/a&gt; for a Zero Data Retention amendment.&lt;/li&gt;
        &lt;/ul&gt;
        &lt;div class="meta"&gt;&lt;strong&gt;Note:&lt;/strong&gt; Even under ZDR, content flagged by a provider's safety classifier may be retained longer (Anthropic up to 2 years) for abuse-monitoring purposes.&lt;/div&gt;

        &lt;h3&gt;Your responsibility&lt;/h3&gt;
        &lt;div class="meta"&gt;PracSim Solutions cannot guarantee any third-party provider's actual practices, and provider policies change over time. &lt;strong&gt;Before sending sensitive, proprietary, regulated, or confidential information through any provider, verify their current terms yourself.&lt;/strong&gt; The information in this panel reflects our research as of this product release and is not a substitute for your own review.&lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Knowledge --&gt;
      &lt;div class="sg" data-sg="knowledge"&gt;
        &lt;h2&gt;Knowledge&lt;/h2&gt;
        &lt;div class="placeholder-block"&gt;Knowledge base browser coming in v0.2 - browse, search, and manage the markdown topics ModelerAI consults before acting. For now, topics live in the &lt;code&gt;KNOWLEDGE/topics/&lt;/code&gt; folder of your install dir.&lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Diagnostics --&gt;
      &lt;div class="sg" data-sg="diagnostics"&gt;
        &lt;h2&gt;Diagnostics&lt;/h2&gt;
        &lt;div class="meta"&gt;Set &lt;code&gt;MODELER_AI_BRIDGE_LOG=1&lt;/code&gt; in your environment before launching FlexSim to enable full envelope tracing. Logs land in &lt;code&gt;%APPDATA%\PRACSIM\ModelerAI\bridge.log&lt;/code&gt;.&lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;&lt;div class="name"&gt;Bridge log&lt;/div&gt;&lt;/div&gt;
          &lt;button id="diag-log"&gt;Open bridge.log&lt;/button&gt;
        &lt;/div&gt;
        &lt;div class="row"&gt;
          &lt;div class="label"&gt;
            &lt;div class="name"&gt;Diagnostics snapshot&lt;/div&gt;
            &lt;div class="desc"&gt;Copies a JSON blob with version + state info to the clipboard for bug reports.&lt;/div&gt;
          &lt;/div&gt;
          &lt;button id="diag-copy"&gt;Copy diagnostics&lt;/button&gt;
        &lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- Remote viewer --&gt;
      &lt;div class="sg" data-sg="remote"&gt;
        &lt;h2&gt;Remote viewer&lt;/h2&gt;
        &lt;div class="meta"&gt;Lets phones on your LAN see and drive this chat. Activated via &lt;code&gt;/remote on&lt;/code&gt;.&lt;/div&gt;
        &lt;div id="remote-section-body"&gt;
          &lt;p&gt;Remote server is off. Use &lt;code&gt;/remote on&lt;/code&gt; in the chat to start it.&lt;/p&gt;
        &lt;/div&gt;
      &lt;/div&gt;

      &lt;!-- About --&gt;
      &lt;div class="sg" data-sg="about"&gt;
        &lt;h2&gt;About&lt;/h2&gt;
        &lt;div class="meta"&gt;
          &lt;p&gt;&lt;strong&gt;PracSim ModelerAI&lt;/strong&gt; v0.1&lt;/p&gt;
          &lt;p&gt;Built by Claude (an AI assistant) under direction from Josh
             at &lt;a href="https://pracsimsolutions.com" target="_blank" rel="noopener"&gt;Practical Simulation Solutions&lt;/a&gt;,
             as a hobby project. Released MIT.&lt;/p&gt;
          &lt;p&gt;&amp;copy; 2026 Practical Simulation Solutions &amp;middot; Released under the
             &lt;a href="https://opensource.org/licenses/MIT" target="_blank" rel="noopener"&gt;MIT License&lt;/a&gt;.
             See &lt;code&gt;LICENSE.txt&lt;/code&gt; in the install directory for the full text.&lt;/p&gt;
          &lt;p&gt;&lt;strong&gt;Questions, suggestions, or just curious?&lt;/strong&gt; Email Josh at
             &lt;a href="mailto:josh@pracsimsolutions.com"&gt;josh@pracsimsolutions.com&lt;/a&gt; —
             happy to hear from you.&lt;/p&gt;
          &lt;p&gt;Ships with two vendored dependencies (both MIT): nlohmann/json for
             JSON parsing, cpp-httplib for the LAN remote-viewer server.&lt;/p&gt;
        &lt;/div&gt;
      &lt;/div&gt;
    &lt;/div&gt;
  &lt;/section&gt;

  &lt;div id="remote-disconnect-overlay" role="dialog" aria-modal="true"&gt;
    &lt;div id="remote-disconnect-card"&gt;
      &lt;h3 id="remote-disconnect-title"&gt;Reconnecting…&lt;/h3&gt;
      &lt;p id="remote-disconnect-msg"&gt;Trying to reach the server.&lt;/p&gt;
      &lt;button id="remote-disconnect-retry"&gt;Retry now&lt;/button&gt;
    &lt;/div&gt;
  &lt;/div&gt;

  &lt;script&gt;
    (function () {
      'use strict';

      // ---- remote / local detection ----
      // In the FlexSim CEF shell, fireFlexsimEvent exists. In any other
      // browser (phone, desktop browser tab), it doesn't — that's a strong
      // "we are remote" signal. MUST run BEFORE the mock-shim install below,
      // since that shim assigns fireFlexsimEvent and would falsify the check.
      var IS_REMOTE = typeof fireFlexsimEvent !== 'function';

      // ---- phone-mode detection ----
      // Phone-mode = stripped UI (transcript + compose + Stop, no settings,
      // no model picker, no provider picker). It's applied:
      //   1. Automatically when IS_REMOTE — any non-local-CEF client (phone,
      //      browser tab) gets the stripped view; they have no business
      //      configuring host settings anyway (server-side guards already
      //      reject host-only envelopes from remote subscribers).
      //   2. Explicitly when ?mode=phone is in the URL — lets us test phone-
      //      mode in the FlexSim CEF panel too without standing up a remote
      //      client.
      // Done synchronously before first paint so display:none takes effect
      // before host-only controls have a chance to render.
      (function () {
        var params = new URLSearchParams(window.location.search);
        if (IS_REMOTE || params.get('mode') === 'phone') {
          document.body.classList.add('phone-mode');
        }
      })();

      // On remote first load, the token comes in via URL fragment
      // (preferred — never reaches a server) or query string (fallback).
      // We extract, store, and clean the URL bar.
      var REMOTE_TOKEN = '';
      // Stable subscriber id — generated client-side, pinned in
      // localStorage, sent as `X-Mraisid` on EVERY remote fetch.
      // We do this because mobile Safari (and some other browsers) silently
      // drop our HttpOnly+SameSite cookie over plain-HTTP+IP-address origins,
      // which broke /api/send (no cookie → 401 NO_SUBSCRIBER) even though
      // /api/poll appeared to work (poll path registers a fresh subscriber
      // on every cookieless request, so state flowed phone-ward but every
      // outbound phone-to-FlexSim send 401'd).
      var REMOTE_SID = '';
      if (IS_REMOTE) {
        try {
          var hash = window.location.hash || '';
          var qs   = window.location.search || '';
          var m = hash.match(/token=([^&amp;]+)/) || qs.match(/[?&amp;]token=([^&amp;]+)/);
          if (m) {
            REMOTE_TOKEN = decodeURIComponent(m[1]);
            window.localStorage.setItem('modelerai_remote_token', REMOTE_TOKEN);
            // Strip the token from the URL bar — protects against
            // screenshots / bookmarks that would leak it.
            window.history.replaceState(null, '', window.location.pathname);
          } else {
            REMOTE_TOKEN = window.localStorage.getItem('modelerai_remote_token') || '';
          }
          REMOTE_SID = window.localStorage.getItem('modelerai_remote_sid') || '';
          if (!REMOTE_SID) {
            // 12-char base36 id. Token-bearer auth gates writes regardless,
            // so this just has to be unique-enough per device.
            var rnd = (Math.random().toString(36).slice(2, 8)
                       + Date.now().toString(36)).slice(0, 12);
            REMOTE_SID = 'remote-js-' + rnd;
            window.localStorage.setItem('modelerai_remote_sid', REMOTE_SID);
          }
        } catch (e) { /* localStorage disabled — sid + token stay empty */ }
      }

      if (IS_REMOTE) {
        fetch('/api/health')
          .then(function (r) { return r.json(); })
          .then(function (j) {
            if (!j.ok) console.warn('remote: /api/health returned not-ok', j);
          })
          .catch(function (err) {
            // Server unreachable — token storage is fine but UX should
            // show an error. Reconnect overlay will catch this on the
            // first poll anyway.
            console.warn('remote: /api/health unreachable', err);
          });
      }

      // ----- bridge fallback for browser sanity-check -----
      if (typeof fireFlexsimEvent === 'undefined') {
        window.fireFlexsimEvent = function () {
          var args = Array.prototype.slice.call(arguments);
          var cb = (typeof args[args.length - 1] === 'function') ? args.pop() : null;
          console.warn('[mock] fireFlexsimEvent', args);
          if (cb) setTimeout(function () { cb(''); }, 0);
        };
      }

      // ----- settings cache -----
      // The DLL is the source of truth for all persisted settings (global
      // settings.json + per-project ModelerAI/variables/projectSettings).
      // The viewer caches the most recent state_update.settings.effective
      // and reads from that. Writes go through set_setting envelopes.
      // localStorage is only used for the one-shot migration of legacy
      // values from the pre-settings days; afterwards it stays empty.
      var effectiveSettings = {
        theme:              'system',
        send_on_enter:      true,
        show_debug_info:    false,
        session_strategy:   'per-file',
        manual_session_key: '',
        active_model_id:    '',
        active_provider_id: 'anthropic',
        effort:             'off',
        mode:               'ask',
        tool_policies:      {}
      };
      var settingsProjectAvail = false;

      function getSessionStrategy()  { return effectiveSettings.session_strategy || 'per-file'; }
      function getCustomSessionKey() { return effectiveSettings.manual_session_key || ''; }

      function persistSetting(key, value, scope) {
        // scope: "global" (default for most things) or "project" (per-.fsm)
        sendEnvelope('set_setting', { key: key, value: value, scope: scope || 'global' });
      }

      // ----- theme -----
      // Source of truth lives in the DLL's global settings.json. The
      // initial paint uses whatever localStorage had (legacy migration
      // path) so we don't flash the wrong theme; once the DLL pushes
      // settings on viewer_ready, we re-apply the canonical value.
      function applyTheme(t) {
        document.documentElement.classList.remove('light', 'dark', 'system');
        document.documentElement.classList.add(t || 'system');
      }
      var legacyTheme = (function () {
        try { return localStorage.getItem('modelerai.theme') || 'system'; } catch (e) { return 'system'; }
      })();
      applyTheme(legacyTheme);
      document.querySelectorAll('input[name="theme"]').forEach(function (r) {
        r.checked = (r.value === legacyTheme);
        r.addEventListener('change', function () {
          if (r.checked) {
            applyTheme(r.value);
            persistSetting('theme', r.value, 'global');
          }
        });
      });

      // ----- tab switching -----
      var tabs = document.querySelectorAll('.tab');
      var panes = {
        chat:     document.getElementById('pane-chat'),
        settings: document.getElementById('pane-settings')
      };
      function activateTab(name) {
        tabs.forEach(function (x) { x.classList.toggle('active', x.dataset.tab === name); });
        Object.keys(panes).forEach(function (k) {
          panes[k].classList.toggle('active', k === name);
        });
      }
      tabs.forEach(function (t) {
        t.addEventListener('click', function () { activateTab(t.dataset.tab); });
      });

      // ----- mobile hamburger toggle -----
      // On phones the tabbar shows a hamburger that toggles between the
      // chat and settings panes; same as tapping the Settings/Chat tab
      // but easier to find with a thumb.
      var mobToggle = document.getElementById('mobile-settings-toggle');
      if (mobToggle) {
        mobToggle.addEventListener('click', function () {
          var settingsActive = panes.settings &amp;&amp; panes.settings.classList.contains('active');
          activateTab(settingsActive ? 'chat' : 'settings');
        });
      }

      // ----- settings sidebar -----
      var sbItems = document.querySelectorAll('.sb-item');
      var sgPanels = document.querySelectorAll('.sg');
      sbItems.forEach(function (item) {
        item.addEventListener('click', function () {
          sbItems.forEach(function (x) { x.classList.remove('active'); });
          item.classList.add('active');
          var key = item.dataset.sg;
          sgPanels.forEach(function (p) { p.classList.toggle('active', p.dataset.sg === key); });
          // Auto-refresh the saved-sessions list when the user opens
          // Session Memory — the list could be stale if other tabs ran
          // turns that produced new saves.
          if (key === 'sessions') {
            try { refreshManualSessionList(); } catch (e) {}
          }
        });
      });

      // (Trust badge was removed from the chat header in the toolbar-rework
       // pass. Privacy posture now lives only in Settings → Privacy. The DLL
       // still pushes effective_settings updates that include trust info,
       // but no in-chat surface consumes them.)

      // ----- mode selector (Ask / Plan / Apply) -----
      // Mode lives DLL-side as the source of truth. We mirror it locally for
      // UI state + send mode_change envelopes when the user picks a new mode
      // from the popover. The DLL echoes back a `state_update.mode` we honor
      // on viewer_ready.
      var currentMode = 'ask';
      var currentEffort = 'off';

      var MODE_DESCRIPTIONS = {
        ask: {
          title: 'Ask',
          desc:  'Read-only chat. Mutating tools are hidden from the model. Safe default.',
          icon:  '&lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;&lt;circle cx="12" cy="12" r="9"/&gt;&lt;path d="M9.5 9a2.5 2.5 0 0 1 5 0c0 1.5-2.5 1.5-2.5 4"/&gt;&lt;circle cx="12" cy="17" r="0.6" fill="currentColor"/&gt;&lt;/svg&gt;'
        },
        plan: {
          title: 'Plan',
          desc:  'The model can see all tools and describe what it would do, but mutating calls are intercepted and do not run.',
          icon:  '&lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;&lt;path d="M3 6h13M3 12h9M3 18h13"/&gt;&lt;circle cx="19" cy="6" r="1.4" fill="currentColor"/&gt;&lt;circle cx="15" cy="12" r="1.4" fill="currentColor"/&gt;&lt;circle cx="19" cy="18" r="1.4" fill="currentColor"/&gt;&lt;/svg&gt;'
        },
        apply: {
          title: 'Apply',
          desc:  'The model can use all tools for real. Mutating calls pop an approval modal first (until you "Always allow" a tool).',
          icon:  '&lt;svg viewBox="0 0 24 24" aria-hidden="true"&gt;&lt;path d="M13 2L3 14h8l-1 8 10-12h-8z"/&gt;&lt;/svg&gt;'
        }
      };

      var EFFORT_LEVELS = ['off', 'low', 'medium', 'high'];

      function applyModeUI(mode) {
        currentMode = mode || 'ask';
        var lab = document.getElementById('mode-btn-label');
        if (lab) lab.textContent = MODE_DESCRIPTIONS[currentMode] ? MODE_DESCRIPTIONS[currentMode].title : 'Ask';
        // If the popover is currently open, re-render so the checkmark moves.
        var pop = document.getElementById('mode-popover');
        if (pop &amp;&amp; pop.classList.contains('open')) renderModePopover();
      }
      function applyEffortUI(effort) {
        currentEffort = effort || 'off';
        var pop = document.getElementById('mode-popover');
        if (pop &amp;&amp; pop.classList.contains('open')) renderModePopover();
      }

      function renderModePopover() {
        var pop = document.getElementById('mode-popover');
        if (!pop) return;
        var html = '';
        html += '&lt;div class="mode-pop-header"&gt;Modes&lt;span class="mp-hint"&gt;click to switch&lt;/span&gt;&lt;/div&gt;';
        ['ask','plan','apply'].forEach(function (m) {
          var def = MODE_DESCRIPTIONS[m];
          html += '&lt;div class="mode-pop-row' + (m === currentMode ? ' selected' : '') + '" data-mode="' + m + '"&gt;'
                + '&lt;span class="mp-icon"&gt;' + def.icon + '&lt;/span&gt;'
                + '&lt;div class="mp-body"&gt;'
                +   '&lt;div class="mp-title"&gt;' + def.title + '&lt;/div&gt;'
                +   '&lt;div class="mp-desc"&gt;' + def.desc + '&lt;/div&gt;'
                + '&lt;/div&gt;'
                + '&lt;span class="mp-check"&gt;✓&lt;/span&gt;'
                + '&lt;/div&gt;';
        });
        // Effort row — radio-dial style. Each dot is clickable to jump to
        // that specific level (off/low/medium/high). The active level's dot
        // is the only one with .active styling. The popover STAYS open on
        // dot click so the user can experiment; outside-click closes it.
        var dots = '';
        for (var k = 0; k &lt; EFFORT_LEVELS.length; k++) {
          var lvl = EFFORT_LEVELS[k];
          var isActive = (lvl === currentEffort);
          dots += '&lt;span class="me-dot' + (isActive ? ' active' : '')
               +  '" data-effort="' + lvl + '"'
               +  ' title="' + lvl + '"&gt;&lt;/span&gt;';
        }
        html += '&lt;div class="mode-pop-effort" id="mode-effort-row"&gt;'
              + '&lt;span class="me-label"&gt;Effort&lt;/span&gt;'
              + '&lt;span class="me-name"&gt;(' + currentEffort + ')&lt;/span&gt;'
              + '&lt;span class="me-dots"&gt;' + dots + '&lt;/span&gt;'
              + '&lt;/div&gt;';
        pop.innerHTML = html;
        pop.querySelectorAll('.mode-pop-row').forEach(function (row) {
          row.addEventListener('click', function () {
            var m = row.getAttribute('data-mode');
            if (!m || m === currentMode) { closeModePopover(); return; }
            applyModeUI(m);
            dispatchEnvelope(JSON.stringify({
              t: 'mode_change', id: newId(), p: { mode: currentMode }
            }));
            closeModePopover();
          });
        });
        pop.querySelectorAll('.me-dot').forEach(function (dot) {
          dot.addEventListener('click', function (ev) {
            ev.stopPropagation();
            var lvl = dot.getAttribute('data-effort');
            if (!lvl || lvl === currentEffort) return;
            applyEffortUI(lvl);
            sendEnvelope('set_setting', { key: 'effort', value: lvl, scope: 'global' });
            // No closeModePopover — keep open so the user can keep tinkering.
          });
        });
      }
      function positionModePopover() {
        var btn = document.getElementById('mode-btn');
        var pop = document.getElementById('mode-popover');
        if (!btn || !pop) return;
        var r = btn.getBoundingClientRect();
        // Render to measure, then place ABOVE the button (composer is at
        // the bottom of the viewport).
        pop.style.visibility = 'hidden';
        pop.classList.add('open');
        var ph = pop.getBoundingClientRect().height;
        pop.style.top  = Math.max(8, r.top - ph - 6) + 'px';
        pop.style.left = r.left + 'px';
        pop.style.visibility = '';
      }
      function openModePopover() {
        renderModePopover();
        positionModePopover();
        var btn = document.getElementById('mode-btn');
        if (btn) btn.classList.add('open');
      }
      function closeModePopover() {
        var pop = document.getElementById('mode-popover');
        var btn = document.getElementById('mode-btn');
        if (pop) pop.classList.remove('open');
        if (btn) btn.classList.remove('open');
      }
      (function () {
        var btn = document.getElementById('mode-btn');
        var pop = document.getElementById('mode-popover');
        if (!btn || !pop) return;
        btn.addEventListener('click', function (ev) {
          ev.stopPropagation();
          if (pop.classList.contains('open')) closeModePopover();
          else openModePopover();
        });
        document.addEventListener('click', function (ev) {
          if (!pop.classList.contains('open')) return;
          if (pop.contains(ev.target) || btn.contains(ev.target)) return;
          closeModePopover();
        });
        window.addEventListener('resize', function () {
          if (pop.classList.contains('open')) positionModePopover();
        });
      })();

      // ----- chat state -----
      var messagesEl = document.getElementById('messages');
      var composerEl = document.getElementById('composer');
      var inputEl    = document.getElementById('input');
      var sendBtnEl  = document.getElementById('send');
      var stopBtnEl  = document.getElementById('stop');
      var popoverEl  = document.getElementById('slash-popover');

      function setSendingState(sending) {
        if (sending) {
          sendBtnEl.style.display = 'none';
          stopBtnEl.style.display = '';
          setThinking('Thinking…');
        } else {
          sendBtnEl.style.display = '';
          stopBtnEl.style.display = 'none';
          hideThinking();
        }
      }

      // -------- Thinking indicator --------
      // Shows a pulsing dot + status label + elapsed seconds while a turn
      // is in flight. The dot is a CSS keyframe animation, NOT a JS
      // setInterval render, so it keeps moving even if the main thread
      // is briefly blocked by a heavy JSON parse / markdown render. That
      // makes it a reliable "the page is still alive" signal — the one
      // bit of feedback the user has been missing.
      //
      // setThinking(label) — show with text label, start elapsed timer
      // updateThinking(label) — update text, keep timer
      // hideThinking() — hide and reset timer
      //
      // We update the label as the turn progresses:
      //   start                 -&gt; "Thinking…"
      //   first assistant_chunk -&gt; "Streaming response…"
      //   tool_call_start       -&gt; "Running {name}…"
      //   tool batch settles    -&gt; "Thinking after tools…" (deferred)
      var thinkingEl        = document.getElementById('thinking-indicator');
      var thinkingLabelEl   = document.getElementById('thinking-label');
      var thinkingElapsedEl = document.getElementById('thinking-elapsed');
      var thinkingTimer     = null;
      var thinkingStartTime = 0;

      function fmtElapsed(secs) {
        if (secs &lt; 60) return secs + 's';
        var m = Math.floor(secs / 60);
        var s = secs % 60;
        return m + 'm ' + (s &lt; 10 ? '0' : '') + s + 's';
      }

      function setThinking(label) {
        if (!thinkingEl) return;
        thinkingLabelEl.textContent = label || 'Thinking…';
        thinkingEl.classList.add('show');
        if (thinkingTimer) return; // already running
        thinkingStartTime = Date.now();
        thinkingElapsedEl.textContent = '0s';
        thinkingTimer = setInterval(function () {
          var secs = Math.floor((Date.now() - thinkingStartTime) / 1000);
          thinkingElapsedEl.textContent = fmtElapsed(secs);
        }, 1000);
      }
      function updateThinking(label) {
        if (!thinkingEl || !thinkingEl.classList.contains('show')) return;
        if (label) thinkingLabelEl.textContent = label;
      }
      function hideThinking() {
        if (!thinkingEl) return;
        thinkingEl.classList.remove('show');
        if (thinkingTimer) { clearInterval(thinkingTimer); thinkingTimer = null; }
        thinkingStartTime = 0;
      }

      stopBtnEl.addEventListener('click', function () {
        if (!activeTurnId) return;
        var env = JSON.stringify({ t: 'cancel_turn', id: activeTurnId, p: { turn_id: activeTurnId } });
        dispatchEnvelope(env);
        stopBtnEl.disabled = true;
        stopBtnEl.textContent = 'Stopping...';
      });

      // ----- attachments (images) ----------------------------------------
      // The user can add images via three paths: clicking +, pasting from the
      // clipboard, or drag-dropping into the composer. Each accepted image
      // gets a chip above the textarea. CURRENT STATE: chips are kept in
      // browser memory only — the send path doesn't yet forward image bytes
      // to the Anthropic provider (that needs DLL-side bridge protocol
      // changes to carry binary blobs and an image_block emitter in the
      // anthropic adapter). For now images give visual feedback so the user
      // can build up + remove attachments; the backend wiring is a separate
      // slice. A console warning fires on send so it's not silent.
      var attachments = [];   // [{ id, name, dataUrl, width, height, sizeBytes }]
      var nextAttachId = 1;

      var attachBtnEl     = document.getElementById('attach-btn');
      var attachInputEl   = document.getElementById('attach-input');
      var attachmentsRow  = document.getElementById('attachments-row');
      var toastEl         = document.getElementById('composer-toast');

      function bytesShort(n) {
        if (n &lt; 1024)        return n + ' B';
        if (n &lt; 1024 * 1024) return Math.round(n / 1024) + ' KB';
        return (n / 1024 / 1024).toFixed(1) + ' MB';
      }

      // Anthropic accepts only these media types for image content blocks.
      // Filter at the UI layer so the user sees the rejection immediately.
      var ANTHROPIC_IMAGE_MIME = {
        'image/jpeg': 1, 'image/jpg': 1, 'image/png': 1,
        'image/gif':  1, 'image/webp': 1
      };

      var toastTimer = null;
      function showToast(msg, kind) {
        if (!toastEl) return;
        toastEl.textContent = msg;
        toastEl.style.borderColor = kind === 'error'
            ? '#c0392b'
            : 'var(--accent)';
        toastEl.classList.add('show');
        if (toastTimer) clearTimeout(toastTimer);
        toastTimer = setTimeout(function () {
          toastEl.classList.remove('show');
        }, 3200);
      }
      function renderAttachments() {
        if (!attachmentsRow) return;
        attachmentsRow.innerHTML = '';
        attachments.forEach(function (a) {
          var chip = document.createElement('div');
          chip.className = 'attach-chip';
          chip.title = a.name + ' · ' + bytesShort(a.sizeBytes || 0);

          var thumb = document.createElement('img');
          thumb.className = 'chip-thumb';
          thumb.src = a.dataUrl;
          thumb.alt = '';
          chip.appendChild(thumb);

          var nm = document.createElement('span');
          nm.className = 'chip-name';
          nm.textContent = a.name;
          chip.appendChild(nm);

          if (a.width &amp;&amp; a.height) {
            var dim = document.createElement('span');
            dim.className = 'chip-dim';
            dim.textContent = a.width + 'x' + a.height;
            chip.appendChild(dim);
          }

          var rm = document.createElement('button');
          rm.type = 'button';
          rm.className = 'chip-remove';
          rm.textContent = '×';
          rm.title = 'Remove this attachment';
          rm.addEventListener('click', function () {
            attachments = attachments.filter(function (x) { return x.id !== a.id; });
            renderAttachments();
          });
          chip.appendChild(rm);

          attachmentsRow.appendChild(chip);
        });
      }

      function addImageFile(file) {
        if (!file || !file.type) return;
        // Mime filter: Anthropic only accepts a small set. Surface the
        // rejection so the user knows why nothing happened.
        var mt = (file.type || '').toLowerCase();
        if (!ANTHROPIC_IMAGE_MIME[mt]) {
          showToast('Unsupported image type "' + (file.type || 'unknown')
                    + '". Use PNG, JPEG, GIF, or WebP.', 'error');
          return;
        }
        // Anthropic's documented hard limit is 5 MB encoded; base64 adds
        // ~33% overhead, so the raw file ceiling that fits cleanly is ~3.7 MB.
        // Soft-warn at 3.5 MB and hard-reject above 5 MB raw to be safe.
        if (file.size &amp;&amp; file.size &gt; 5 * 1024 * 1024) {
          showToast('Image "' + file.name + '" is ' + bytesShort(file.size)
                    + ' — Anthropic limits images to ~5 MB. Resize and try again.', 'error');
          return;
        }
        if (file.size &amp;&amp; file.size &gt; 3.5 * 1024 * 1024) {
          showToast('Heads up: "' + file.name + '" is ' + bytesShort(file.size)
                    + '; close to Anthropic\'s 5 MB cap after base64 encoding.', 'warn');
        }
        var reader = new FileReader();
        reader.onload = function (ev) {
          var dataUrl = ev.target.result;
          var probe = new Image();
          probe.onload = function () {
            attachments.push({
              id:        nextAttachId++,
              name:      file.name || ('clipboard-image-' + nextAttachId + '.png'),
              dataUrl:   dataUrl,
              width:     probe.naturalWidth,
              height:    probe.naturalHeight,
              sizeBytes: file.size || 0
            });
            renderAttachments();
          };
          probe.onerror = function () {
            // Couldn't decode — still add the chip so the user sees something
            // happened, just without dimensions.
            attachments.push({
              id:        nextAttachId++,
              name:      file.name || 'image',
              dataUrl:   dataUrl,
              width:     0, height: 0,
              sizeBytes: file.size || 0
            });
            renderAttachments();
          };
          probe.src = dataUrl;
        };
        reader.readAsDataURL(file);
      }

      if (attachBtnEl &amp;&amp; attachInputEl) {
        attachBtnEl.addEventListener('click', function () { attachInputEl.click(); });
        attachInputEl.addEventListener('change', function () {
          var files = attachInputEl.files;
          if (!files) return;
          for (var i = 0; i &lt; files.length; i++) addImageFile(files[i]);
          // Reset so picking the same file again still fires `change`.
          attachInputEl.value = '';
        });
      }

      // Paste: when the input has focus and the clipboard contains image
      // data, capture it as an attachment instead of dumping a base64 string
      // into the textarea.
      inputEl.addEventListener('paste', function (ev) {
        if (!ev.clipboardData || !ev.clipboardData.items) return;
        var got = false;
        for (var i = 0; i &lt; ev.clipboardData.items.length; i++) {
          var item = ev.clipboardData.items[i];
          if (item.kind === 'file' &amp;&amp; item.type &amp;&amp; item.type.indexOf('image/') === 0) {
            var f = item.getAsFile();
            if (f) { addImageFile(f); got = true; }
          }
        }
        if (got) ev.preventDefault();
      });

      // Drag-drop anywhere on the composer accepts images. Prevent the
      // browser's default "open the file" behavior on the whole window
      // so a missed drop doesn't navigate away. Drag-over flips the
      // composer's .drag-over class for the visual pulse.
      var dragDepth = 0;  // dragenter/leave fire on children too; count to handle correctly
      composerEl.addEventListener('dragenter', function (ev) {
        ev.preventDefault(); ev.stopPropagation();
        if (ev.dataTransfer &amp;&amp; ev.dataTransfer.types &amp;&amp;
            Array.prototype.indexOf.call(ev.dataTransfer.types, 'Files') &gt;= 0) {
          dragDepth++;
          composerEl.classList.add('drag-over');
        }
      });
      composerEl.addEventListener('dragover', function (ev) {
        ev.preventDefault(); ev.stopPropagation();
      });
      composerEl.addEventListener('dragleave', function (ev) {
        ev.preventDefault(); ev.stopPropagation();
        dragDepth = Math.max(0, dragDepth - 1);
        if (dragDepth === 0) composerEl.classList.remove('drag-over');
      });
      composerEl.addEventListener('drop', function (ev) {
        ev.preventDefault(); ev.stopPropagation();
        dragDepth = 0;
        composerEl.classList.remove('drag-over');
        var dt = ev.dataTransfer;
        if (!dt || !dt.files) return;
        for (var i = 0; i &lt; dt.files.length; i++) addImageFile(dt.files[i]);
      });
      // Suppress the browser default for stray drops outside the composer
      // (drag a file from Explorer into anywhere) so they don't navigate.
      window.addEventListener('dragover', function (ev) { ev.preventDefault(); });
      window.addEventListener('drop',     function (ev) { ev.preventDefault(); });

      // Slash toolbar button — convenience for users who don't know they
      // can just type "/". Focuses the input and, if it isn't already
      // starting with /, prepends one. Then fires `input` so the existing
      // slash-popover renderer picks it up.
      //
      // CRITICAL: stopPropagation. Without it, the click bubbles to the
      // document-level outside-click handler at the bottom of the slash
      // wiring, which would call closeSlash() in the same tick we opened
      // it. (The handler exempts inputEl + popoverEl but the slash button
      // is neither; the exemption for #slash-btn lives down there too as
      // a defensive backup.)
      //
      // ALSO CRITICAL: the inputEl `blur` handler at the bottom of the slash
      // wiring queues a `setTimeout(closeSlash, 100)`. When the textarea
      // has focus and the user clicks the slash button, the button's
      // mousedown briefly blurs the input — so a closeSlash is already
      // queued by the time our click handler re-focuses and opens the
      // popover. The shared `slashJustOpenedByButton` flag below lets the
      // blur handler bail out for one tick when this happens.
      var slashBtnEl = document.getElementById('slash-btn');
      var slashJustOpenedByButton = false;
      if (slashBtnEl) {
        // mousedown fires BEFORE blur, so we can set the flag in time.
        slashBtnEl.addEventListener('mousedown', function () {
          slashJustOpenedByButton = true;
        });
        slashBtnEl.addEventListener('click', function (ev) {
          ev.stopPropagation();
          inputEl.focus();
          if (inputEl.value.charAt(0) !== '/') {
            inputEl.value = '/' + inputEl.value;
          }
          inputEl.selectionStart = inputEl.selectionEnd = 1;
          inputEl.dispatchEvent(new Event('input'));
          // Clear the flag after a short window — long enough that the
          // queued blur-close has run (and been suppressed), short enough
          // that subsequent intentional blurs still close as designed.
          setTimeout(function () { slashJustOpenedByButton = false; }, 200);
        });
      }

      // ----- composer splitter (drag to resize the input area) -----
      // Lives between the messages area and the composer. Mouse-down on
      // the bar starts a drag; mouse moves on the document adjust the
      // textarea's height; mouse-up releases. Min 38px (single-line),
      // max 70vh so the input can't completely eat the messages list.
      var splitterEl = document.getElementById('composer-splitter');
      var splitterDragging   = false;
      var splitterStartY     = 0;
      var splitterStartHeight = 0;

      function inputCurrentHeight() {
        // Use getBoundingClientRect for the rendered pixel height — survives
        // padding / border quirks that .offsetHeight sometimes gets wrong.
        return inputEl.getBoundingClientRect().height;
      }

      function clampInputHeight(px) {
        var minH = 38;
        var maxH = Math.max(minH + 1, Math.floor(window.innerHeight * 0.7));
        if (px &lt; minH) return minH;
        if (px &gt; maxH) return maxH;
        return px;
      }

      splitterEl.addEventListener('mousedown', function (ev) {
        ev.preventDefault();
        splitterDragging = true;
        splitterStartY = ev.clientY;
        splitterStartHeight = inputCurrentHeight();
        splitterEl.classList.add('dragging');
        document.body.classList.add('composer-resizing');
      });

      document.addEventListener('mousemove', function (ev) {
        if (!splitterDragging) return;
        // Drag UP (smaller clientY) = grow input. Drag DOWN = shrink.
        var delta = splitterStartY - ev.clientY;
        var next = clampInputHeight(splitterStartHeight + delta);
        inputEl.style.height = next + 'px';
      });

      document.addEventListener('mouseup', function () {
        if (!splitterDragging) return;
        splitterDragging = false;
        splitterEl.classList.remove('dragging');
        document.body.classList.remove('composer-resizing');
      });

      // Double-click resets to the single-line default. Handy if the user
      // dragged it way too tall.
      splitterEl.addEventListener('dblclick', function () {
        inputEl.style.height = '38px';
      });

      // ----- onboarding wizard -----
      // Steps in order. Last is "done" (Finish button replaces Next).
      var onbSteps = ['welcome', 'key', 'cap', 'model', 'done'];
      var onbCurrent = 0;

      function showOnboarding() {
        var overlay = document.getElementById('onb-overlay');
        if (!overlay) return;
        overlay.classList.add('shown');
        onbCurrent = 0;
        applyOnbStep();
      }
      function hideOnboarding() {
        var overlay = document.getElementById('onb-overlay');
        if (overlay) overlay.classList.remove('shown');
      }
      function applyOnbStep() {
        var stepKey = onbSteps[onbCurrent];
        // Stepper chip states.
        document.querySelectorAll('.onb-step').forEach(function (s) {
          var idx = onbSteps.indexOf(s.dataset.step);
          s.classList.remove('active','done');
          if (idx === onbCurrent) s.classList.add('active');
          else if (idx &lt; onbCurrent) s.classList.add('done');
        });
        // Pane visibility.
        document.querySelectorAll('.onb-pane').forEach(function (p) {
          p.hidden = (p.dataset.step !== stepKey);
        });
        // Buttons.
        document.getElementById('onb-back').disabled = (onbCurrent === 0);
        var lastStep = (onbCurrent === onbSteps.length - 1);
        document.getElementById('onb-next').hidden = lastStep;
        document.getElementById('onb-finish').hidden = !lastStep;
        // Model step: reflect current active model.
        if (stepKey === 'model') reflectOnbModelPick();
      }
      function reflectOnbModelPick() {
        var current = (activeModel &amp;&amp; activeModel.model_id) || '';
        document.querySelectorAll('.onb-pick').forEach(function (b) {
          b.classList.toggle('picked', b.dataset.model === current);
        });
        var line = document.getElementById('onb-model-current');
        if (line) line.textContent = 'Current selection: '
          + (activeModel ? (activeModel.model_name + ' (' + activeModel.model_id + ')') : '(none)');
      }
      document.getElementById('onb-next').addEventListener('click', function () {
        if (onbCurrent &lt; onbSteps.length - 1) { onbCurrent++; applyOnbStep(); }
      });
      document.getElementById('onb-back').addEventListener('click', function () {
        if (onbCurrent &gt; 0) { onbCurrent--; applyOnbStep(); }
      });
      document.getElementById('onb-skip').addEventListener('click', function () {
        // Skip still counts as "completed" — we don't want to nag the
        // user every launch if they actively dismissed the wizard.
        sendEnvelope('complete_onboarding', {});
        hideOnboarding();
      });
      document.getElementById('onb-finish').addEventListener('click', function () {
        sendEnvelope('complete_onboarding', {});
        hideOnboarding();
      });
      // API-key save inside the wizard reuses save_api_key + a small UI status line.
      document.getElementById('onb-key-save').addEventListener('click', function () {
        var key = document.getElementById('onb-key').value.trim();
        var st  = document.getElementById('onb-key-status');
        if (!key) {
          st.className = 'onb-key-status err meta';
          st.textContent = 'Paste the key (starts with sk-ant-) above first.';
          return;
        }
        sendEnvelope('save_api_key', { provider: 'anthropic', key: key });
        document.getElementById('onb-key').value = '';
        st.className = 'onb-key-status ok meta';
        st.textContent = 'Key saved (DPAPI-encrypted). You can verify in Settings → Providers.';
      });
      // Model pick buttons inside wizard route through the existing provider_switch.
      document.querySelectorAll('.onb-pick').forEach(function (btn) {
        btn.addEventListener('click', function () {
          var modelId = btn.dataset.model;
          sendEnvelope('provider_switch', { provider: 'anthropic', model: modelId });
          // Optimistic UI: mark this card picked until the active_model push confirms.
          document.querySelectorAll('.onb-pick').forEach(function (b) { b.classList.remove('picked'); });
          btn.classList.add('picked');
        });
      });

      // ----- slash autocomplete state -----
      // Populated from state_update slash_commands; fallback hardcoded list
      // mirrors the DLL catalog so the popover works pre-mount.
      var slashCatalog = [
        { name: 'help',   aliases: ['?'],              description: 'Show available commands',
          details: 'Lists every slash command. Pass a name for details: /help clear.',
          usage: '/help [command]',
          args: { placeholder: 'command', choices: [
            {value:'help',   label:'/help',   description:'Show available commands'},
            {value:'clear',  label:'/clear',  description:'Wipe conversation history'},
            {value:'cost',   label:'/cost',   description:'Show token usage and cost'},
            {value:'model',  label:'/model',  description:'Show or switch the active model'},
            {value:'effort', label:'/effort', description:'Set extended-thinking budget'},
            {value:'export', label:'/export', description:'Save conversation as markdown'}
          ] } },
        { name: 'clear',  aliases: ['new', 'reset'],   description: 'Wipe conversation history',
          details: 'Drops the entire conversation from memory and zeroes the session token counters.',
          usage: '/clear' },
        { name: 'cost',   aliases: ['tokens','usage'], description: 'Show token usage and cost',
          details: 'Displays running token usage and dollar estimate at current model rates.',
          usage: '/cost' },
        { name: 'model',  aliases: [],                 description: 'Show or switch the active model',
          details: 'Without an argument, reports the currently active model. With a model id, switches to it.',
          usage: '/model [haiku|sonnet|opus]',
          args: { placeholder: 'model', choices: [
            {value:'claude-haiku-4-5',  label:'Haiku 4.5',  description:'Cheapest + fastest ($1/$5 per Mtok)'},
            {value:'claude-sonnet-4-6', label:'Sonnet 4.6', description:'Balanced default ($3/$15 per Mtok)'},
            {value:'claude-opus-4-7',   label:'Opus 4.7',   description:'Smartest ($15/$75 per Mtok)'}
          ] } },
        { name: 'effort', aliases: [],                 description: 'Set extended-thinking budget',
          details: 'Higher effort = more silent reasoning per turn = better answers on hard questions, at extra cost.',
          usage: '/effort off|low|medium|high',
          args: { placeholder: 'level', choices: [
            {value:'off',    label:'off',    description:'Disable extended thinking (default)'},
            {value:'low',    label:'low',    description:'~2k thinking tokens — quick reasoning'},
            {value:'medium', label:'medium', description:'~8k thinking tokens — standard'},
            {value:'high',   label:'high',   description:'~16k thinking tokens — heavy reasoning'}
          ] } },
        { name: 'export', aliases: ['save'],           description: 'Save conversation as markdown',
          details: 'Writes the conversation as a timestamped markdown file under %APPDATA%/PRACSIM/ModelerAI/exports/.',
          usage: '/export' },
        { name: 'tools',  aliases: [],                 description: 'List the tools the model can call',
          details: 'Shows every registered tool the LLM can choose to invoke this turn.',
          usage: '/tools' }
      ];
      // Mode is 'cmd' (selecting a command) or 'arg' (picking from a
      // command's choices). Switches when the user types ' ' after a
      // known command.
      var slashMode = 'cmd';
      var slashActiveCmd = null;  // when slashMode === 'arg'
      var slashFiltered = [];
      var slashSelectedIdx = 0;

      var pollTimer    = null;
      var activeBubble = null;
      var activeTurnId = null;
      var activeTurnIsSlash = false;  // true when the current turn is a /command reply
      var placeholderEl = messagesEl.querySelector('.placeholder');

      function newId() {
        return Date.now().toString(36) + Math.random().toString(36).slice(2, 10);
      }
      function clearPlaceholder() {
        if (placeholderEl &amp;&amp; placeholderEl.parentNode) {
          placeholderEl.parentNode.removeChild(placeholderEl);
          placeholderEl = null;
        }
      }
      // ----- minimal markdown renderer (offline, no deps) -----
      // Handles: fenced code blocks, inline code, bold, italic, links
      // (http/https only), headings, unordered lists, paragraphs. Strips
      // raw HTML so model output can't inject script. Good enough for
      // chat replies; advanced features (tables, nested lists, math) can
      // be added later if Claude actually emits them.
      function escapeHtml(s) {
        return String(s).replace(/[&amp;&lt;&gt;"']/g, function (c) {
          return {'&amp;':'&amp;amp;','&lt;':'&amp;lt;','&gt;':'&amp;gt;','"':'&amp;quot;',"'":'&amp;#39;'}[c];
        });
      }
      function renderMarkdown(src) {
        if (!src) return '';
        // Pull fenced code blocks out first so the rest of the rewriter
        // can't transform their contents.
        var codeBlocks = [];
        src = src.replace(/```([a-zA-Z0-9_+\-]*)\n?([\s\S]*?)```/g, function (_, lang, code) {
          codeBlocks.push({ lang: lang || '', code: code });
          return '\0\0CODE' + (codeBlocks.length - 1) + '\0\0';
        });
        src = escapeHtml(src);
        // Inline code — single backtick, single line.
        src = src.replace(/`([^`\n]+)`/g, '&lt;code&gt;$1&lt;/code&gt;');
        // Bold / italic (greedy but bounded to non-asterisk runs).
        src = src.replace(/\*\*([^*\n]+)\*\*/g, '&lt;strong&gt;$1&lt;/strong&gt;');
        src = src.replace(/\*([^*\n]+)\*/g, '&lt;em&gt;$1&lt;/em&gt;');
        // Links — http/https only. Open in a new window (CEF will route to default browser).
        src = src.replace(/\[([^\]\n]+)\]\((https?:\/\/[^\s)]+)\)/g,
          '&lt;a href="$2" target="_blank" rel="noopener noreferrer"&gt;$1&lt;/a&gt;');
        // Headings.
        src = src.replace(/^### (.+)$/gm, '&lt;h4&gt;$1&lt;/h4&gt;');
        src = src.replace(/^## (.+)$/gm,  '&lt;h3&gt;$1&lt;/h3&gt;');
        src = src.replace(/^# (.+)$/gm,   '&lt;h2&gt;$1&lt;/h2&gt;');

        // Tables — GitHub-flavored markdown tables. Detect a header row
        // followed by a separator row (--- |---) followed by 1+ body rows.
        // Pipes can be escaped as \| if Claude ever emits them in cell text.
        src = src.replace(/(^\|.+\|[ \t]*\n\|[ \t]*[-:| \t]+[ \t]*\n(?:\|.+\|[ \t]*\n?)+)/gm,
          function (block) {
            var lines = block.trim().split('\n');
            if (lines.length &lt; 2) return block;
            var splitRow = function (row) {
              row = row.replace(/^\|/, '').replace(/\|[ \t]*$/, '');
              return row.split(/(?&lt;!\\)\|/).map(function (c) {
                return c.replace(/\\\|/g, '|').trim();
              });
            };
            var header = splitRow(lines[0]);
            var body   = lines.slice(2).map(splitRow);
            var html = '&lt;table class="md-table"&gt;&lt;thead&gt;&lt;tr&gt;';
            header.forEach(function (h) { html += '&lt;th&gt;' + h + '&lt;/th&gt;'; });
            html += '&lt;/tr&gt;&lt;/thead&gt;&lt;tbody&gt;';
            body.forEach(function (r) {
              html += '&lt;tr&gt;';
              r.forEach(function (c) { html += '&lt;td&gt;' + c + '&lt;/td&gt;'; });
              html += '&lt;/tr&gt;';
            });
            html += '&lt;/tbody&gt;&lt;/table&gt;\n';
            return html;
          });

        // Blockquotes — &gt; prefix; consecutive &gt;-prefixed lines join into one quote.
        src = src.replace(/(?:^|\n)((?:&amp;gt; .+\n?)+)/g, function (_, items) {
          var inner = items.trim().split('\n').map(function (l) {
            return l.replace(/^&amp;gt; /, '');
          }).join('&lt;br&gt;');
          return '\n&lt;blockquote&gt;' + inner + '&lt;/blockquote&gt;\n';
        });

        // Ordered lists — consecutive lines starting with digits + .
        // Greedy regex so a list of 12 items stays one block.
        src = src.replace(/(?:^|\n)((?:\d+\. .+\n?)+)/g, function (_, items) {
          var lines = items.trim().split('\n').map(function (l) {
            return '&lt;li&gt;' + l.replace(/^\d+\. /, '') + '&lt;/li&gt;';
          }).join('');
          return '\n&lt;ol&gt;' + lines + '&lt;/ol&gt;\n';
        });

        // Unordered lists — consecutive lines starting with - or *.
        src = src.replace(/(?:^|\n)((?:[-*] .+\n?)+)/g, function (_, items) {
          var lines = items.trim().split('\n').map(function (l) {
            return '&lt;li&gt;' + l.replace(/^[-*] /, '') + '&lt;/li&gt;';
          }).join('');
          return '\n&lt;ul&gt;' + lines + '&lt;/ul&gt;\n';
        });
        // Paragraphs — blank-line separated, skip blocks already wrapped.
        var parts = src.split(/\n\s*\n/);
        src = parts.map(function (p) {
          p = p.trim();
          if (!p) return '';
          if (/^&lt;(ul|ol|h\d|pre|blockquote|table)/.test(p)) return p;
          return '&lt;p&gt;' + p.replace(/\n/g, '&lt;br&gt;') + '&lt;/p&gt;';
        }).join('\n');
        // Re-insert code blocks (escaped contents).
        src = src.replace(/\0\0CODE(\d+)\0\0/g, function (_, i) {
          var cb = codeBlocks[parseInt(i, 10)];
          return '&lt;pre&gt;&lt;code class="lang-' + escapeHtml(cb.lang) + '"&gt;'
                  + escapeHtml(cb.code) + '&lt;/code&gt;&lt;/pre&gt;';
        });
        return src;
      }

      // Last user-message text — Regenerate refires this verbatim.
      var lastUserText = '';

      function attachBubbleActions(el, role) {
        // Hover-revealed toolbar inside each bubble. Assistant bubbles
        // get Copy + Regenerate; user bubbles get Copy only; system /
        // error / tool get nothing (they're transient and copying them
        // rarely makes sense).
        if (role !== 'assistant' &amp;&amp; role !== 'user') return;
        var bar = document.createElement('div');
        bar.className = 'bubble-actions';

        var copyBtn = document.createElement('button');
        copyBtn.className = 'bubble-action';
        copyBtn.title = 'Copy message text';
        copyBtn.textContent = 'Copy';
        copyBtn.addEventListener('click', function () {
          var raw = el.getAttribute('data-md') || el.textContent || '';
          if (navigator.clipboard &amp;&amp; navigator.clipboard.writeText) {
            navigator.clipboard.writeText(raw).then(function () {
              copyBtn.textContent = 'Copied';
              setTimeout(function () { copyBtn.textContent = 'Copy'; }, 1200);
            });
          }
        });
        bar.appendChild(copyBtn);

        if (role === 'assistant') {
          var regenBtn = document.createElement('button');
          regenBtn.className = 'bubble-action';
          regenBtn.title = 'Re-ask the same question as a new turn. (Note: this appends a fresh reply; full "rewind last turn" arrives with undo support.)';
          regenBtn.textContent = 'Regenerate';
          regenBtn.addEventListener('click', function () {
            if (!lastUserText || activeTurnId) return;
            // Re-fire the most-recent user message as a new turn.
            inputEl.value = lastUserText;
            composerEl.dispatchEvent(new Event('submit', { cancelable: true }));
          });
          bar.appendChild(regenBtn);
        }

        el.appendChild(bar);
      }

      // Auto-scroll helper. Force-scrolling to bottom on every new chunk
      // fights the user when they intentionally scroll up to read earlier
      // content during a running turn. Snap to bottom only if the user
      // is already within `threshold` pixels of it; otherwise leave the
      // scroll position alone. 80px = roughly one bubble's worth of slack.
      function isNearBottom(threshold) {
        var t = (typeof threshold === 'number') ? threshold : 80;
        return (messagesEl.scrollHeight - messagesEl.scrollTop - messagesEl.clientHeight) &lt;= t;
      }
      function scrollIfNearBottom(wasNearBottom) {
        // Caller can pass the pre-append bottom state to avoid the
        // common race where adding a tall bubble pushes us "away from
        // the bottom" before the check runs. If omitted, we use the
        // current state.
        var should = (typeof wasNearBottom === 'boolean') ? wasNearBottom : isNearBottom();
        if (should) messagesEl.scrollTop = messagesEl.scrollHeight;
      }

      function appendBubble(role, text, opts) {
        clearPlaceholder();
        var el = document.createElement('div');
        el.className = 'bubble bubble-' + role;
        // Content goes in a child div so the actions toolbar can live as
        // a sibling without being wiped by innerHTML updates during streaming.
        var content = document.createElement('div');
        content.className = 'bubble-content';
        // Markdown for assistant + system (slash replies). Plain text for
        // user / error / tool — they're either user input echo or system
        // statuses that shouldn't be re-interpreted.
        if (role === 'assistant' || role === 'system') {
          el.setAttribute('data-md', text || '');
          content.innerHTML = renderMarkdown(text || '');
        } else {
          content.textContent = text || '';
        }
        el.appendChild(content);

        // User-attached images: render as thumbnail strip below the text.
        // Click-through opens the image at native size in a new window.
        if (role === 'user' &amp;&amp; opts &amp;&amp; opts.attachments &amp;&amp; opts.attachments.length) {
          var strip = document.createElement('div');
          strip.className = 'bubble-attachments';
          opts.attachments.forEach(function (a) {
            if (!a || !a.dataUrl) return;
            var img = document.createElement('img');
            img.className = 'bubble-attachment';
            img.src = a.dataUrl;
            img.alt = a.name || 'image';
            img.title = (a.name || 'image') + (a.width ? ' · ' + a.width + 'x' + a.height : '');
            img.addEventListener('click', function () {
              // Open full-size in a new tab/window for inspection.
              try { window.open(a.dataUrl, '_blank'); } catch (e) {}
            });
            strip.appendChild(img);
          });
          el.appendChild(strip);
        }

        if (role === 'user') lastUserText = text || '';
        attachBubbleActions(el, role);
        var wasAtBottom = isNearBottom();
        messagesEl.appendChild(el);
        // User-authored bubbles always snap to bottom (they just hit send,
        // they want to see their message). For assistant / tool / system
        // streamed bubbles, only auto-scroll if they were already at the
        // bottom — leaves them in place when scrolled up to read.
        if (role === 'user') messagesEl.scrollTop = messagesEl.scrollHeight;
        else                 scrollIfNearBottom(wasAtBottom);
        return el;
      }

      // Paint a QR + URL card into a remote_status bubble. Reads from
      // window._remoteStatus (kept up to date by the remote_status
      // state_update branch in applyStateUpdate). Tagged via
      // data-remote-status="1" so applyStateUpdate can re-render the
      // QR if the status changes after the bubble was placed (e.g. the
      // user toggled the remote server on/off while the bubble was on
      // screen, or rotated the token).
      function renderRemoteStatusBubble(bubbleEl) {
        if (!bubbleEl) return;
        // Mark the bubble FIRST so the post-state_update sweep
        // ([data-remote-status="1"]) can find it even if the bubble
        // was created before _remoteStatus arrived. Envelope ordering
        // is assistant_chunk -&gt; assistant_done -&gt; state_update, so the
        // bubble is always placed before we have data to paint into it.
        bubbleEl.setAttribute('data-remote-status', '1');
        if (!window._remoteStatus) return;
        var st = window._remoteStatus;
        if (!st.enabled) {
          var c0 = bubbleEl.querySelector('.bubble-content');
          if (c0) c0.innerHTML = '&lt;p&gt;&lt;strong&gt;📱 Remote disabled&lt;/strong&gt;&lt;/p&gt;';
          bubbleEl.setAttribute('data-remote-status', '1');
          return;
        }
        var card = document.createElement('div');
        card.style.cssText = 'text-align:center;padding:8px;';
        if (st.primary_url) {
          var qrDiv = document.createElement('div');
          qrDiv.style.cssText = 'display:inline-block;background:white;padding:8px;border-radius:8px;';
          try {
            var qr = qrcode(0, 'M');
            qr.addData(st.primary_url);
            qr.make();
            qrDiv.innerHTML = qr.createImgTag(5, 8);
          } catch (e) {
            qrDiv.textContent = '[QR render failed: ' + e.message + ']';
          }
          card.appendChild(qrDiv);
          var label = document.createElement('div');
          label.style.cssText = 'margin-top:6px;font-size:12px;opacity:0.8;';
          label.textContent = st.primary_label || '';
          card.appendChild(label);
          var urlRow = document.createElement('div');
          urlRow.style.cssText = 'margin-top:8px;font-family:monospace;font-size:12px;';
          urlRow.innerHTML = '&lt;a href="' + st.primary_url + '" target="_blank"&gt;'
                           + st.primary_url + '&lt;/a&gt;';
          card.appendChild(urlRow);
        }
        var content = bubbleEl.querySelector('.bubble-content');
        if (content) {
          content.innerHTML = '&lt;p&gt;&lt;strong&gt;📱 Remote enabled&lt;/strong&gt;&lt;/p&gt;';
          content.appendChild(card);
        }
        bubbleEl.setAttribute('data-remote-status', '1');
      }

      // Cache of the most recent active_model payload so the model picker
      // can highlight + the header pill can refresh on dropdown changes.
      var activeModel = null;
      var availableModels = [];

      function refreshModelPill() {
        if (!activeModel) return;
        // Update the bottom-toolbar Model button label. We show just the
        // model name (short) — the provider + effort live in the popover
        // and the trust posture lives in Settings → Privacy.
        var lab = document.getElementById('model-btn-label');
        if (lab) lab.textContent = activeModel.model_name || activeModel.model_id || 'Model';
        // Mirror effort into the mode-popover state so the dots reflect
        // the active level without waiting for a separate setting echo.
        if (typeof activeModel.effort === 'string') applyEffortUI(activeModel.effort);
        renderModelPickerPopover();   // keep dropdown in sync with active
      }

      // -------- Inline model picker popover (chat header) --------
      // Click the model pill to open a dropdown of every known model
      // across providers. Click an entry to switch — sends a
      // provider_switch envelope, same path Settings -&gt; Model uses.
      function modelCostHint(m) {
        // Compact "in/out" $/Mtok pair. Hide if pricing is unknown.
        var i = m.input_usd_per_mtok;
        var o = m.output_usd_per_mtok;
        if (typeof i !== 'number' || typeof o !== 'number' || (i === 0 &amp;&amp; o === 0)) return '';
        return '$' + i + '/' + o + ' per Mtok';
      }
      function renderModelPickerPopover() {
        var pop = document.getElementById('model-picker-popover');
        if (!pop) return;
        if (!availableModels.length) {
          pop.innerHTML = '&lt;div class="mp-group-label"&gt;No models available&lt;/div&gt;';
          return;
        }
        // Group by provider so it's scannable when multiple providers
        // are configured.
        var byProvider = {};
        availableModels.forEach(function (m) {
          var key = m.provider_name || m.provider_id || 'Unknown';
          if (!byProvider[key]) byProvider[key] = [];
          byProvider[key].push(m);
        });
        var activeId = activeModel ? activeModel.model_id : '';
        var html = '';
        Object.keys(byProvider).forEach(function (provName) {
          html += '&lt;div class="mp-group-label"&gt;' + provName + '&lt;/div&gt;';
          byProvider[provName].forEach(function (m) {
            var isActive = m.model_id === activeId;
            html += '&lt;div class="mp-item' + (isActive ? ' active' : '') +
                    '" data-pid="' + (m.provider_id || '') +
                    '" data-mid="' + m.model_id + '"&gt;' +
                    '&lt;span class="mp-check"&gt;' + (isActive ? '✓' : '') + '&lt;/span&gt;' +
                    '&lt;span class="mp-name"&gt;' + (m.model_name || m.model_id) + '&lt;/span&gt;' +
                    '&lt;span class="mp-cost"&gt;' + modelCostHint(m) + '&lt;/span&gt;' +
                    '&lt;/div&gt;';
          });
        });
        pop.innerHTML = html;
        // Wire each item's click. Re-bound on every render since innerHTML
        // wipes prior handlers.
        pop.querySelectorAll('.mp-item').forEach(function (el) {
          el.addEventListener('click', function () {
            var pid = el.getAttribute('data-pid');
            var mid = el.getAttribute('data-mid');
            sendEnvelope('provider_switch', { provider: pid, model: mid });
            closeModelPicker();
          });
        });
      }
      function positionModelPicker() {
        var btn = document.getElementById('model-btn');
        var pop = document.getElementById('model-picker-popover');
        if (!btn || !pop) return;
        var r = btn.getBoundingClientRect();
        // Render first to measure height, then place ABOVE the button so it
        // rises into the messages area rather than overlapping the toolbar.
        pop.style.visibility = 'hidden';
        pop.classList.add('open');
        var ph = pop.getBoundingClientRect().height;
        pop.style.top  = Math.max(8, r.top - ph - 6) + 'px';
        pop.style.left = r.left + 'px';
        pop.style.visibility = '';
      }
      function openModelPicker() {
        var btn = document.getElementById('model-btn');
        var pop = document.getElementById('model-picker-popover');
        if (!btn || !pop) return;
        renderModelPickerPopover();
        positionModelPicker();
        pop.classList.add('open');
        btn.classList.add('open');
      }
      function closeModelPicker() {
        var btn = document.getElementById('model-btn');
        var pop = document.getElementById('model-picker-popover');
        if (!pop || !btn) return;
        pop.classList.remove('open');
        btn.classList.remove('open');
      }
      (function () {
        var btn = document.getElementById('model-btn');
        var pop = document.getElementById('model-picker-popover');
        if (!btn || !pop) return;
        btn.addEventListener('click', function (ev) {
          ev.stopPropagation();
          if (pop.classList.contains('open')) closeModelPicker();
          else openModelPicker();
        });
        document.addEventListener('click', function (ev) {
          if (!pop.classList.contains('open')) return;
          if (pop.contains(ev.target) || btn.contains(ev.target)) return;
          closeModelPicker();
        });
        window.addEventListener('resize', function () {
          if (pop.classList.contains('open')) positionModelPicker();
        });
      })();
      // Tag each provider with a known-trust tier. Vetted providers ship
      // green; custom providers default to amber (treat unknown as risky);
      // DeepSeek-host detection elevates to red; localhost endpoints to
      // local. Sync with the table in Settings -&gt; Privacy.
      function classifyProvider(pid, baseUrl) {
        var built = { anthropic: 1, openai: 1, groq: 1, mistral: 1,
                      together: 1, fireworks: 1 };
        if (pid &amp;&amp; built[pid.toLowerCase()]) {
          return { tier: 'green', label: 'Privacy: No-Training API' };
        }
        var url = (baseUrl || '').toLowerCase();
        if (url.indexOf('deepseek') &gt;= 0) {
          return { tier: 'red',   label: 'Privacy: HIGH RISK (DeepSeek)' };
        }
        if (url.indexOf('localhost') &gt;= 0 || url.indexOf('127.0.0.1') &gt;= 0 ||
            url.indexOf('://0.0.0.0') &gt;= 0) {
          return { tier: 'local', label: 'Privacy: Local — never leaves machine' };
        }
        if (pid) {
          return { tier: 'amber', label: 'Privacy: Custom — verify terms' };
        }
        return { tier: 'unknown', label: 'Privacy: …' };
      }
      function refreshTrustBadge() {
        // Trust pill was removed from the chat header — privacy posture now
        // lives only in Settings → Privacy. Kept as a no-op so existing
        // call sites (refreshModelPill, possible future surfaces) don't
        // need conditional checks.
      }
      function refreshModelDropdown() {
        var sel = document.getElementById('model-select');
        if (!sel) return;
        if (!availableModels.length &amp;&amp; activeModel) {
          sel.innerHTML = '&lt;option value="' + (activeModel.model_id || '') + '"&gt;'
            + (activeModel.model_name || '') + '&lt;/option&gt;';
          sel.disabled = true;
          return;
        }
        sel.disabled = false;
        sel.innerHTML = '';
        // Group by provider so the picker reads naturally: Anthropic
        // models under one header, each custom provider under its own.
        var groups = {};
        var order  = [];
        availableModels.forEach(function (m) {
          var pid = m.provider_id || 'unknown';
          if (!groups[pid]) {
            groups[pid] = { name: m.provider_name || pid, items: [] };
            order.push(pid);
          }
          groups[pid].items.push(m);
        });
        order.forEach(function (pid) {
          var g = groups[pid];
          var og = document.createElement('optgroup');
          og.label = g.name;
          g.items.forEach(function (m) {
            var opt = document.createElement('option');
            opt.value = m.model_id;
            var costPart = (m.input_usd_per_mtok || m.output_usd_per_mtok)
              ? '  ($' + (m.input_usd_per_mtok || 0).toFixed(2)
                + '/$' + (m.output_usd_per_mtok || 0).toFixed(2) + ' Mtok)'
              : '  (local / free)';
            opt.textContent = m.model_name + costPart;
            if (activeModel &amp;&amp; m.model_id === activeModel.model_id) opt.selected = true;
            og.appendChild(opt);
          });
          sel.appendChild(og);
        });
      }
      function refreshModelCapsAndPricing() {
        if (!activeModel) return;
        var caps = document.getElementById('model-caps');
        caps.innerHTML = '';
        if (activeModel.supports_tools)    caps.innerHTML += '&lt;span class="badge badge-ok"&gt;Tools&lt;/span&gt; ';
        if (activeModel.supports_vision)   caps.innerHTML += '&lt;span class="badge badge-ok"&gt;Vision&lt;/span&gt; ';
        if (activeModel.supports_thinking) caps.innerHTML += '&lt;span class="badge badge-ok"&gt;Thinking&lt;/span&gt; ';
        caps.innerHTML += '&lt;span class="badge badge-grey"&gt;ctx=' + (activeModel.context_tokens || 0).toLocaleString() + '&lt;/span&gt;';

        var pricingEl = document.getElementById('model-pricing');
        if (pricingEl) {
          pricingEl.textContent = '$' + (activeModel.input_usd_per_mtok || 0).toFixed(2)
            + '/Mtok input · $' + (activeModel.output_usd_per_mtok || 0).toFixed(2) + '/Mtok output';
        }
      }
      function refreshEffortDropdown() {
        var sel = document.getElementById('effort-select');
        if (!sel || !activeModel) return;
        sel.value = activeModel.effort || 'off';
        sel.disabled = !activeModel.supports_thinking;
      }

      // ----- state_update handler -----
      // Badge tooltip ages update only on remote_status pushes (which fire
      // from /remote on/off/rotate/status). For a constantly-fresh "Ns ago"
      // view, open the Settings → Remote section (which re-fetches when
      // shown).
      function refreshRemoteBadge() {
        var el = document.getElementById('remote-badge');
        if (!el) return;
        var st = window._remoteStatus || {};
        var clients = st.connected_clients || [];
        if (!st.enabled || clients.length === 0) {
          el.style.display = 'none';
          return;
        }
        el.style.display = 'inline-flex';
        el.textContent   = '📱 ' + clients.length;
        var lines = clients.map(function (c) {
          return c.id.slice(0, 12) + ' · ' + (c.ip || '?') + ' · '
               + (c.last_seen_seconds_ago || 0) + 's';
        });
        el.title = lines.join('\n');
      }
      function refreshRemoteSettings() {
        var body = document.getElementById('remote-section-body');
        if (!body) return;
        var st = window._remoteStatus || {};
        if (!st.enabled) {
          body.innerHTML = '&lt;p&gt;Remote server is off. Use &lt;code&gt;/remote on&lt;/code&gt; '
                         + 'in the chat to start it.&lt;/p&gt;';
          return;
        }
        var clients = st.connected_clients || [];
        var allUrls = st.all_urls || [];

        var html = '';
        html += '&lt;div class="row"&gt;&lt;strong&gt;Port:&lt;/strong&gt;&amp;nbsp;' + st.port + '&lt;/div&gt;';
        html += '&lt;div class="row"&gt;&lt;strong&gt;Token:&lt;/strong&gt;&amp;nbsp;'
             + (st.token_short || '') + '&lt;/div&gt;';
        html += '&lt;div class="row"&gt;&lt;strong&gt;Uptime:&lt;/strong&gt;&amp;nbsp;'
             + Math.round(st.uptime_seconds || 0) + 's&lt;/div&gt;';

        html += '&lt;h3&gt;Connected clients (' + clients.length + ')&lt;/h3&gt;';
        if (!clients.length) {
          html += '&lt;div class="meta"&gt;None yet.&lt;/div&gt;';
        } else {
          html += '&lt;ul class="remote-client-list"&gt;';
          clients.forEach(function (c) {
            html += '&lt;li&gt;' + c.id.slice(0, 12) + ' · ' + (c.ip || '?')
                  + ' · ' + (c.last_seen_seconds_ago || 0) + 's ago '
                  + '&lt;button class="remote-kick" data-id="'
                  + c.id + '"&gt;Disconnect&lt;/button&gt;&lt;/li&gt;';
          });
          html += '&lt;/ul&gt;';
        }

        html += '&lt;h3&gt;All LAN URLs&lt;/h3&gt;&lt;ul class="remote-url-list"&gt;';
        allUrls.forEach(function (u) {
          html += '&lt;li&gt;' + u.label + ': &lt;code&gt;' + u.url + '&lt;/code&gt;'
                + ' &lt;button class="remote-copy" data-url="' + u.url + '"&gt;Copy&lt;/button&gt;&lt;/li&gt;';
        });
        html += '&lt;/ul&gt;';

        html += '&lt;div class="row"&gt;'
             + '&lt;button id="remote-rotate-btn"&gt;Rotate token&lt;/button&gt;&amp;nbsp;'
             + '&lt;button id="remote-stop-btn"&gt;Stop server&lt;/button&gt;&lt;/div&gt;';

        body.innerHTML = html;

        body.querySelectorAll('.remote-kick').forEach(function (b) {
          b.addEventListener('click', function () {
            sendEnvelope('remote_kick', { subscriber_id: b.getAttribute('data-id') });
          });
        });
        body.querySelectorAll('.remote-copy').forEach(function (b) {
          b.addEventListener('click', function () {
            try { navigator.clipboard.writeText(b.getAttribute('data-url')); } catch (e) {}
          });
        });
        var rot = document.getElementById('remote-rotate-btn');
        if (rot) rot.addEventListener('click', function () {
          sendEnvelope('user_message', { text: '/remote rotate' });
        });
        var stop = document.getElementById('remote-stop-btn');
        if (stop) stop.addEventListener('click', function () {
          sendEnvelope('user_message', { text: '/remote off' });
        });
      }
      function applyStateUpdate(key, value) {
        if (key === 'active_model') {
          activeModel = value;
          refreshModelPill();
          refreshModelDropdown();
          refreshModelCapsAndPricing();
          refreshEffortDropdown();
        }
        else if (key === 'available_models') {
          availableModels = Array.isArray(value) ? value : [];
          refreshModelDropdown();
        }
        else if (key === 'effort') {
          if (activeModel) activeModel.effort = value.effort || 'off';
          refreshModelPill();
          refreshEffortDropdown();
        }
        else if (key === 'session_stats') {
          document.getElementById('stat-input').textContent      = (value.input_tokens || 0).toLocaleString();
          document.getElementById('stat-output').textContent     = (value.output_tokens || 0).toLocaleString();
          document.getElementById('stat-cache-read').textContent = (value.cache_read_input_tokens || 0).toLocaleString();
          document.getElementById('stat-turns').textContent      = (value.turns || 0).toLocaleString();
          // Prefer DLL-computed cost (accurate per-turn at active-model rates).
          // Fall back to client-side estimate only if cost_usd isn't present.
          var cost = (typeof value.cost_usd === 'number') ? value.cost_usd :
            (value.input_tokens                 || 0) * 3.00     / 1000000 +
            (value.output_tokens                || 0) * 15.00    / 1000000 +
            (value.cache_read_input_tokens      || 0) * 0.30     / 1000000 +
            (value.cache_creation_input_tokens  || 0) * 3.75     / 1000000;
          document.getElementById('stat-cost').textContent = '$' + cost.toFixed(4);
          // Auto-clear the cost-warning banner when stats drop below the
          // warn threshold (e.g. after /clear). Stay-shown when still over.
          var warnAt = (effectiveSettings &amp;&amp; effectiveSettings.cost_warn_usd) || 0;
          if (warnAt &gt; 0 &amp;&amp; cost &lt; warnAt) {
            var banner = document.getElementById('cost-warning-banner');
            if (banner) banner.style.display = 'none';
          }
        }
        else if (key &amp;&amp; key.indexOf('provider_status.') === 0) {
          var pid = key.substr('provider_status.'.length);
          // Lookup table maps each built-in's provider id to its UI prefix
          // and the env-var label shown in the descriptive line. Anything
          // not in this table is treated as a user-added custom provider
          // and routed to the cp-badge-&lt;id&gt; element.
          var BUILTIN_UI = {
            'anthropic': { prefix: 'anth',    envvar: 'ANTHROPIC_API_KEY' },
            'openai':    { prefix: 'openai',  envvar: 'OPENAI_API_KEY'    },
            'gemini':    { prefix: 'gemini',  envvar: 'GEMINI_API_KEY'    },
            'xai':       { prefix: 'xai',     envvar: 'XAI_API_KEY'       },
            'mistral':   { prefix: 'mistral', envvar: 'MISTRAL_API_KEY'   }
          };
          var meta = BUILTIN_UI[pid];
          if (meta) {
            var badge     = document.getElementById(meta.prefix + '-badge');
            var msg       = document.getElementById(meta.prefix + '-status-msg');
            var forgetBtn = document.getElementById(meta.prefix + '-forget');
            if (!badge || !msg) return;
            badge.className = 'badge ';
            var src = value.key_source || 'none';
            var srcLabel = src === 'keystore'
              ? 'in keystore (DPAPI-encrypted)'
              : (src === 'fallback' ? 'from ' + meta.envvar + ' env var' : '');
            if (value.status === 'authenticated') {
              badge.classList.add('badge-ok');  badge.textContent = 'Authenticated';
              msg.textContent = 'Key validated · ' + srcLabel;
            } else if (value.status === 'invalid') {
              badge.classList.add('badge-bad'); badge.textContent = 'Invalid';
              msg.textContent = (value.message || 'Test failed.') + (srcLabel ? '  ·  ' + srcLabel : '');
            } else if (value.status === 'set_untested') {
              badge.classList.add('badge-warn'); badge.textContent = 'Set (not tested)';
              msg.textContent = 'Click Test key to validate · ' + srcLabel;
            } else {
              badge.classList.add('badge-grey'); badge.textContent = 'Not set';
              msg.textContent = 'Paste your key via Set key, or set ' + meta.envvar + ' in your environment.';
            }
            // Forget only meaningful when keystore actually has it.
            if (forgetBtn) forgetBtn.disabled = (src !== 'keystore');
          } else {
            // User-added custom provider — uses cp-badge-&lt;id&gt; elements
            // rendered by the custom-providers list.
            var badgeEl = document.getElementById('cp-badge-' + pid);
            if (badgeEl) {
              badgeEl.className = 'badge ';
              if (value.status === 'authenticated') {
                badgeEl.classList.add('badge-ok');  badgeEl.textContent = 'Authenticated';
              } else if (value.status === 'invalid') {
                badgeEl.classList.add('badge-bad'); badgeEl.textContent = 'Invalid';
                badgeEl.title = value.message || '';
              } else if (value.status === 'set_untested') {
                badgeEl.classList.add('badge-warn'); badgeEl.textContent = 'Set (not tested)';
              } else {
                badgeEl.classList.add('badge-grey'); badgeEl.textContent = 'Not set';
              }
            }
          }
        }
        else if (key === 'user_rules') {
          var p = document.getElementById('rules-path');
          var pv = document.getElementById('rules-preview');
          p.textContent = value.path || '(path unavailable)';
          pv.textContent = (value.preview || '').trim() || '(empty)';
        }
        else if (key === 'slash_commands') {
          slashCatalog = Array.isArray(value) ? value : [];
        }
        else if (key === 'session') {
          // Session metadata + (optionally) loaded message history.
          var sKeyEl  = document.getElementById('session-key');
          var sFileEl = document.getElementById('session-file');
          var badge   = document.getElementById('session-count');
          if (sKeyEl)  sKeyEl.textContent  = value.key || '(no anchor)';
          if (sFileEl) sFileEl.textContent = value.session_file || '(no path)';
          var n = value.count || 0;
          if (badge) {
            badge.textContent = n + ' message' + (n === 1 ? '' : 's');
            badge.className = 'badge ' + (n &gt; 0 ? 'badge-ok' : 'badge-grey');
          }

          // Show / hide the anchor-missing bar at the top of the chat
          // surface. anchor_missing = true means there's no in-model
          // ModelerAI object, so this conversation is in-memory only
          // until the user runs /addmodelerai.
          var anchorBar = document.getElementById('anchor-bar');
          if (anchorBar) {
            anchorBar.style.display = value.anchor_missing ? 'flex' : 'none';
          }

          // Cache the dir so the "Copy folder path" button works.
          window._sessionsDir = value.sessions_dir || '';

          // Replay loaded history into the chat surface (only when we
          // actually loaded something).
          if (value.loaded &amp;&amp; Array.isArray(value.messages) &amp;&amp; value.messages.length) {
            if (placeholderEl &amp;&amp; placeholderEl.parentNode) {
              placeholderEl.parentNode.removeChild(placeholderEl);
              placeholderEl = null;
            }
            // Idempotent on repeat viewer_ready calls.
            messagesEl.innerHTML = '';
            // Reset transient batch + tool tracking — any open tool group
            // was tied to DOM elements we just wiped.
            closeToolBatch();
            toolItemsByCallId = {};
            value.messages.forEach(function (m) {
              if (m.role === 'user' || m.role === 'assistant') {
                appendBubble(m.role, m.text || '');
              }
            });
          } else if (value.loaded === false) {
            // Strategy change resolved to a key with no history — clear
            // any previously-rendered bubbles so the UI reflects reality.
            messagesEl.innerHTML =
              '&lt;div class="placeholder"&gt;' +
              '&lt;strong&gt;Fresh chat for this scope&lt;/strong&gt;' +
              'No saved conversation for this session key.&lt;br&gt;' +
              'Type &lt;code&gt;/&lt;/code&gt; to see commands, or just ask about your model.' +
              '&lt;/div&gt;';
            placeholderEl = messagesEl.querySelector('.placeholder');
          }
        }
        else if (key === 'mode') {
          // DLL echoed current mode — sync the UI in case it diverged
          // (e.g. on viewer_ready, or after a successful mode_change).
          applyModeUI(value.mode);
        }
        else if (key === 'conversation_export') {
          // Result of an export_conversation request — show outcome + offer
          // to copy the path. The button stays in its temporary "Exporting…"
          // text until we hear back here.
          var btn = document.getElementById('session-export');
          var status = document.getElementById('export-status');
          if (btn) { btn.disabled = false; btn.textContent = 'Export to Markdown'; }
          if (!status) return;
          status.style.display = 'block';
          if (value &amp;&amp; value.ok) {
            var path = value.path || '';
            var bytes = value.bytes || 0;
            var count = value.message_count || 0;
            status.innerHTML = '&lt;strong&gt;Saved.&lt;/strong&gt; ' + count + ' message' +
              (count === 1 ? '' : 's') + ' · ' + bytes.toLocaleString() + ' bytes · ' +
              '&lt;code&gt;' + path.replace(/&lt;/g, '&amp;lt;') + '&lt;/code&gt; · ' +
              '&lt;a href="#" id="export-copy-path"&gt;Copy path&lt;/a&gt;';
            var copyLink = document.getElementById('export-copy-path');
            if (copyLink) {
              copyLink.addEventListener('click', function (e) {
                e.preventDefault();
                if (navigator.clipboard) {
                  navigator.clipboard.writeText(path);
                  copyLink.textContent = 'Copied ✓';
                  setTimeout(function () { copyLink.textContent = 'Copy path'; }, 1500);
                }
              });
            }
          } else {
            var msg = (value &amp;&amp; value.message) || (value &amp;&amp; value.error) || 'unknown error';
            status.innerHTML = '&lt;strong style="color:#c0392b;"&gt;Export failed:&lt;/strong&gt; ' +
              msg.replace(/&lt;/g, '&amp;lt;');
          }
        }
        else if (key === 'turn_in_flight') {
          // The DLL tells us whether a turn is running. Fired on
          // viewer_ready so a panel resize / refresh mid-turn locks the
          // composer back down instead of letting the user send another
          // message into the same turn slot.
          if (value &amp;&amp; value.in_flight) {
            setSendingState(true);
            sendBtnEl.disabled = true;
            inputEl.disabled   = true;
            stopBtnEl.disabled = false;
            startPolling();
          }
          // When in_flight is false we DON'T forcibly re-enable —
          // finishTurn() handles that path and a stale "false" envelope
          // could otherwise race the real assistant_done.
        }
        else if (key === 'settings') {
          // Canonical settings push from the DLL. Updates the in-memory
          // cache + re-syncs every UI element that mirrors a setting.
          if (value &amp;&amp; value.effective) {
            effectiveSettings = value.effective;
          }
          settingsProjectAvail = !!(value &amp;&amp; value.project_avail);

          // Theme.
          applyTheme(effectiveSettings.theme);
          document.querySelectorAll('input[name="theme"]').forEach(function (r) {
            r.checked = (r.value === effectiveSettings.theme);
          });
          // Send-on-Enter / Show-debug.
          var soe = document.getElementById('send-on-enter');
          if (soe) soe.checked = !!effectiveSettings.send_on_enter;
          var sdi = document.getElementById('show-debug');
          if (sdi) sdi.checked = !!effectiveSettings.show_debug_info;
          // Hide debug drawer when the setting flips off; show on next turn otherwise.
          var dbg = document.getElementById('debug-drawer');
          if (dbg &amp;&amp; !effectiveSettings.show_debug_info) dbg.style.display = 'none';
          // Cost-ceiling inputs.
          var cwe = document.getElementById('cost-warn');
          if (cwe) cwe.value = effectiveSettings.cost_warn_usd || '';
          var cse = document.getElementById('cost-stop');
          if (cse) cse.value = effectiveSettings.cost_stop_usd || '';
          // Agent loop cap.
          var mti = document.getElementById('max-tool-iterations');
          if (mti) mti.value = effectiveSettings.max_tool_iterations || 25;
          // Export folder.
          var exd = document.getElementById('export-dir');
          if (exd) exd.value = effectiveSettings.export_dir || '';
          // Session strategy radios.
          document.querySelectorAll('input[name="session_strategy"]').forEach(function (r) {
            r.checked = (r.value === effectiveSettings.session_strategy);
          });
          updateManualPickerVisibility(effectiveSettings.session_strategy);
          // Save-As behavior radios.
          var sab = effectiveSettings.save_as_behavior || 'copy';
          document.querySelectorAll('input[name="save_as_behavior"]').forEach(function (r) {
            r.checked = (r.value === sab);
          });
          // Mode segmented control.
          applyModeUI(effectiveSettings.mode);

          // Onboarding wizard. ModelerAI is MIT-licensed and ships from
          // the GitHub repo (no installer EULA gate, no runtime acceptance
          // wall — see LICENSE.txt). Onboarding still helps new users
          // configure their API key + model on first launch.
          var currentOnb  = (value &amp;&amp; value.onboarding_current_version) || '';
          var doneOnb     = effectiveSettings.onboarding_completed_version || '';
          if (currentOnb &amp;&amp; currentOnb !== doneOnb) {
            showOnboarding();
          }
        }
        else if (key === 'session_save_status') {
          var ind = document.getElementById('save-indicator');
          if (!ind) return;
          ind.classList.remove('show','ok','err');
          if (value &amp;&amp; value.demo_skipped) {
            // Notebook-style "Demo — not saving" notice. Persistent
            // (not auto-faded) so the user notices nothing is being kept.
            ind.classList.add('show','err');
            ind.textContent = 'Demo mode — conversation not saved';
            return;
          }
          if (value &amp;&amp; value.ok) {
            ind.classList.add('show','ok');
            ind.textContent = 'Session saved (' + (value.messages || 0) + ' messages)';
          } else {
            ind.classList.add('show','err');
            ind.textContent = '⚠ Session save failed — see bridge.log';
          }
          // Auto-fade after a few seconds (success only; errors persist
          // until the next save attempt clears them).
          if (value &amp;&amp; value.ok) {
            setTimeout(function () {
              ind.classList.remove('show');
            }, 2500);
          }
        }
        else if (key === 'debug_turn') {
          var drawer = document.getElementById('debug-drawer');
          if (!drawer) return;
          // Only render if the user has show_debug_info on. The DLL
          // already gates the push but defense-in-depth here keeps the
          // viewer state consistent if the user toggles mid-turn.
          if (!effectiveSettings || !effectiveSettings.show_debug_info) {
            drawer.style.display = 'none';
            return;
          }
          drawer.style.display = 'flex';
          var meta = document.getElementById('debug-meta');
          if (meta) {
            meta.textContent =
              (value.provider_id || '?') + ' / ' + (value.active_model_id || '?') +
              ' · mode=' + (value.mode || '?') +
              ' · effort=' + (value.effort || '?') +
              ' · condensed=' + (value.condensed_size_bytes || 0) + ' B';
          }
          var sps = document.getElementById('debug-sp-stable');
          if (sps) sps.textContent = value.system_prompt_stable || '(empty)';
          var spv = document.getElementById('debug-sp-var');
          if (spv) spv.textContent = value.system_prompt_var || '(empty)';
          var lt = document.getElementById('debug-last-turn');
          if (lt) lt.textContent =
            'turn_id: ' + (value.turn_id || '') + '\n' +
            'user (first 120): ' + (value.user_text_first_120 || '');
        }
        else if (key === 'cost_warning') {
          var banner = document.getElementById('cost-warning-banner');
          if (!banner) return;
          // Coerce numerics defensively even though the DLL controls
          // this payload — keeps the banner XSS-safe via Number().toFixed.
          var sofar  = Number((value &amp;&amp; value.sofar)   || 0);
          var warnAt = Number((value &amp;&amp; value.warn_at) || 0);
          var stopAt = Number((value &amp;&amp; value.stop_at) || 0);
          banner.style.display = 'block';
          banner.innerHTML =
            '&lt;strong&gt;Cost warning:&lt;/strong&gt; session has used $' + sofar.toFixed(4) +
            ' (warn at $' + warnAt.toFixed(2) +
            (stopAt &gt; 0 ? ', hard stop at $' + stopAt.toFixed(2) : '') +
            '). Use &lt;code&gt;/clear&lt;/code&gt; to reset session stats.';
        }
        else if (key === 'custom_providers') {
          var list = document.getElementById('custom-providers-list');
          if (!list) return;
          var arr = Array.isArray(value) ? value : [];
          if (arr.length === 0) {
            list.innerHTML = '&lt;div class="meta"&gt;No custom providers configured. Use the form below to add one.&lt;/div&gt;';
            return;
          }
          list.innerHTML = '';
          arr.forEach(function (p) {
            var row = document.createElement('div');
            row.className = 'provider-row';
            var info = document.createElement('div');
            info.className = 'info';
            info.innerHTML =
              '&lt;div class="name"&gt;' + (p.display_name || p.id) + '&lt;/div&gt;' +
              '&lt;div class="desc"&gt;' +
                '&lt;code&gt;' + (p.base_url || '') + '&lt;/code&gt;' +
                '  ·  model &lt;code&gt;' + (p.model_id || '') + '&lt;/code&gt;' +
              '&lt;/div&gt;';
            var badge = document.createElement('span');
            badge.id = 'cp-badge-' + p.id;
            badge.className = 'badge badge-grey';
            badge.textContent = p.has_api_key ? 'Set (not tested)' : 'Not set';
            var testBtn = document.createElement('button');
            testBtn.textContent = 'Test';
            testBtn.title = 'Validate by sending a 1-token completion.';
            testBtn.addEventListener('click', function () {
              testBtn.textContent = 'Testing…';
              sendEnvelope('test_provider', { provider: p.id });
              setTimeout(function () { testBtn.textContent = 'Test'; }, 2000);
            });
            var setBtn = document.createElement('button');
            setBtn.textContent = 'Set key';
            setBtn.title = 'Paste a new API key for this provider. Stored DPAPI-encrypted.';
            setBtn.addEventListener('click', function () {
              var k = prompt('Paste API key for "' + (p.display_name || p.id) + '":\n(stored DPAPI-encrypted, never plaintext on disk)');
              if (k === null) return;  // cancelled
              if (!k) { alert('Empty key — not saved.'); return; }
              sendEnvelope('save_api_key', { provider: p.id, key: k });
            });
            var forgetBtn = document.createElement('button');
            forgetBtn.textContent = 'Forget';
            forgetBtn.className = 'danger';
            forgetBtn.title = 'Remove this provider\'s key from the keystore.';
            forgetBtn.addEventListener('click', function () {
              if (!confirm('Forget the API key for "' + (p.display_name || p.id) + '"?')) return;
              sendEnvelope('forget_api_key', { provider: p.id });
            });
            var delBtn = document.createElement('button');
            delBtn.textContent = 'Delete';
            delBtn.className = 'danger';
            delBtn.title = 'Remove this provider entirely (config + key).';
            delBtn.addEventListener('click', function () {
              if (!confirm('Remove "' + (p.display_name || p.id) + '" entirely?\nThis deletes the config AND the stored key. You can add it back any time.')) return;
              sendEnvelope('remove_custom_provider', { id: p.id });
            });
            row.appendChild(info);
            row.appendChild(badge);
            row.appendChild(testBtn);
            row.appendChild(setBtn);
            row.appendChild(forgetBtn);
            row.appendChild(delBtn);
            list.appendChild(row);
          });
        }
        else if (key === 'add_custom_provider_result') {
          // Inline confirm — the bridge already pushes refreshed lists,
          // but a small visual confirmation is friendlier than silence.
          if (value &amp;&amp; value.ok) {
            var addBtn = document.getElementById('cp-add');
            if (addBtn) {
              var orig = addBtn.textContent;
              addBtn.textContent = '✓ Added';
              addBtn.disabled = true;
              setTimeout(function () { addBtn.textContent = orig; addBtn.disabled = false; }, 1500);
            }
            // Clear the form.
            ['cp-display-name','cp-base-url','cp-api-key','cp-model-id',
             'cp-model-display','cp-input-cost','cp-output-cost'].forEach(function (id) {
              var el = document.getElementById(id);
              if (el) el.value = '';
            });
          }
        }
        else if (key === 'all_sessions') {
          allSessions = Array.isArray(value) ? value : [];
          var sel = document.getElementById('manual-session-select');
          if (sel) {
            var prev = getCustomSessionKey();
            sel.innerHTML = '';
            var none = document.createElement('option');
            none.value = ''; none.textContent = '(none — fall back to per-model)';
            sel.appendChild(none);
            allSessions.forEach(function (s) {
              var opt = document.createElement('option');
              opt.value = s.key;
              var label = s.model_path
                ? (s.model_path.replace(/\\/g, '/').split('/').pop() || s.model_path)
                : '(unknown model)';
              var when = s.last_saved_ms
                ? new Date(s.last_saved_ms).toISOString().slice(0,16).replace('T', ' ')
                : '';
              opt.textContent = label + ' — ' + (s.message_count || 0) + ' msgs' + (when ? ' · ' + when : '');
              if (s.key === prev) opt.selected = true;
              sel.appendChild(opt);
            });
          }
          // Always also refresh the Session Memory rich list.
          renderSessionMemoryList(allSessions);
        }
        else if (key === 'session_delete_result') {
          // Toast-style notice (just an alert for v1 — could become a
          // proper toast later). The all_sessions push that accompanies
          // delete_session / delete_all_sessions already re-rendered the list.
          if (value &amp;&amp; value.message) {
            console.log('[ModelerAI] session_delete_result:', value);
          }
        }
        else if (key === 'remote_status') {
          window._remoteStatus = value || {};
          // If a QR bubble is already on screen, re-render it with the
          // new data (handles state_update arriving after the bubble was
          // placed, or the user toggling remote while the QR is visible).
          var bubbles = document.querySelectorAll('[data-remote-status="1"]');
          bubbles.forEach(function (b) { renderRemoteStatusBubble(b); });
          refreshRemoteBadge();    // implemented in Task 30
          refreshRemoteSettings(); // implemented in Task 32
          // The heartbeat is already started unconditionally at viewer
          // init (see bottom of script). No-op here — kept the call so the
          // intent is visible: we never want the drain heartbeat to stop
          // while the viewer is alive, since other subscribers can send
          // broadcasts (user_message_broadcast etc.) at any time and we
          // need to drain our queue to render them.
          startRemoteHeartbeat();
        }
      }

      // Tool-call rendering — group consecutive tool calls within a turn
      // into a single collapsible bubble. The batch closes when assistant
      // text resumes streaming (which means another logical step is
      // starting) or when the turn ends.
      //
      // toolItemsByCallId maps call_id -&gt; {item DOM element, tool name}
      // so tool_call_done can find the right entry to finalize AND know
      // which name's count to bump.
      var toolItemsByCallId = {};
      var currentToolBatch  = null;   // outer .bubble-tool-batch element
      var currentToolCounts = {};     // { toolName: {ok, fail, pending} }

      function escapeAttr(s) {
        return String(s).replace(/&amp;/g, '&amp;amp;').replace(/&lt;/g, '&amp;lt;').replace(/&gt;/g, '&amp;gt;');
      }

      function updateBatchSummary() {
        if (!currentToolBatch) return;
        var summaryEl = currentToolBatch.querySelector('.tool-batch-counts');
        if (!summaryEl) return;
        var parts = [];
        for (var name in currentToolCounts) {
          var c = currentToolCounts[name];
          var total = c.ok + c.fail + c.pending;
          var status;
          if (c.pending &gt; 0)      status = ' …';
          else if (c.fail &gt; 0)    status = ' (' + c.ok + ' ✓, ' + c.fail + ' ✗)';
          else                    status = ' ✓';
          parts.push(name + ' × ' + total + status);
        }
        summaryEl.textContent = parts.join(' · ');
      }

      // Wires up a tool-call args/result element so the user can click it to
      // toggle between the short inline summary and the full payload (the
      // DLL sends both as args_summary/args_full and summary/summary_full).
      function makeExpandableToolField(el, shortText, fullText) {
        var hasFull = !!(fullText &amp;&amp; fullText !== shortText);
        el.textContent = shortText || '';
        if (!hasFull) return;
        el.classList.add('tool-field-expandable');
        el.dataset.short = shortText || '';
        el.dataset.full  = fullText;
        el.title = 'Click to expand / collapse';
        el.addEventListener('click', function () {
          var expanded = el.classList.toggle('expanded');
          el.textContent = expanded ? el.dataset.full : el.dataset.short;
        });
      }

      function appendToolBubble(name, argsSummary, callId, argsFull) {
        clearPlaceholder();

        // Start a new batch if none is open.
        if (!currentToolBatch) {
          currentToolBatch = document.createElement('div');
          currentToolBatch.className = 'bubble bubble-tool-batch';
          currentToolBatch.innerHTML =
            '&lt;div class="tool-batch-summary"&gt;' +
              '&lt;span class="tool-batch-counts"&gt;&lt;/span&gt;' +
            '&lt;/div&gt;' +
            '&lt;details class="tool-batch-details"&gt;' +
              '&lt;summary&gt;Show individual calls&lt;/summary&gt;' +
              '&lt;div class="tool-batch-list"&gt;&lt;/div&gt;' +
            '&lt;/details&gt;';
          currentToolCounts = {};
          messagesEl.appendChild(currentToolBatch);
        }

        // Append the per-call item.
        var listEl = currentToolBatch.querySelector('.tool-batch-list');
        var item = document.createElement('div');
        item.className = 'tool-batch-item';
        item.innerHTML =
          '&lt;div class="tool-item-head"&gt;' +
            '&lt;strong&gt;&lt;/strong&gt;' +
            '&lt;span class="tool-item-status"&gt;running…&lt;/span&gt;' +
          '&lt;/div&gt;' +
          '&lt;div class="tool-item-args"&gt;&lt;/div&gt;' +
          '&lt;div class="tool-item-result"&gt;&lt;/div&gt;';
        item.querySelector('strong').textContent = name || 'tool';
        makeExpandableToolField(item.querySelector('.tool-item-args'), argsSummary, argsFull);
        listEl.appendChild(item);
        toolItemsByCallId[callId] = { item: item, name: name };

        // Bump pending count.
        if (!currentToolCounts[name]) currentToolCounts[name] = { ok: 0, fail: 0, pending: 0 };
        currentToolCounts[name].pending += 1;
        updateBatchSummary();

        scrollIfNearBottom();
      }

      function finalizeToolBubble(callId, ok, summary, summaryFull) {
        var rec = toolItemsByCallId[callId];
        if (!rec) return;
        var item = rec.item;
        item.classList.add(ok ? 'ok' : 'fail');
        item.querySelector('.tool-item-status').textContent = ok ? '✓' : '✗';
        makeExpandableToolField(item.querySelector('.tool-item-result'), summary, summaryFull);
        delete toolItemsByCallId[callId];

        // Move from pending -&gt; ok/fail.
        var c = currentToolCounts[rec.name];
        if (c) {
          c.pending = Math.max(0, c.pending - 1);
          if (ok) c.ok += 1; else c.fail += 1;
        }
        updateBatchSummary();
      }

      // Close the active batch — call when assistant text resumes or the
      // turn ends. Subsequent tool_call_starts will create a fresh batch.
      function closeToolBatch() {
        currentToolBatch = null;
        currentToolCounts = {};
      }

      // ----- approval modal (mutating-tool ack) -----
      var pendingApproval = null;  // { interaction_id, ... } while modal open

      function showApprovalModal(p) {
        pendingApproval = p;
        document.getElementById('approval-name').textContent =
          (p.name || 'tool') + '(…)';
        var catEl = document.getElementById('approval-cat');
        catEl.textContent = (p.category || 'mutating').replace(/_/g, ' ');
        document.getElementById('approval-desc').textContent =
          p.description || '';
        var args = p.args || '';
        try {
          var parsed = (typeof args === 'string') ? JSON.parse(args) : args;
          args = JSON.stringify(parsed, null, 2);
        } catch (e) { /* leave as-is */ }
        document.getElementById('approval-args').textContent = args;
        document.getElementById('approval-overlay').classList.add('shown');
      }
      function resolveApproval(decision) {
        if (!pendingApproval) return;
        var iid = pendingApproval.interaction_id;
        pendingApproval = null;
        document.getElementById('approval-overlay').classList.remove('shown');
        sendEnvelope('tool_approval_response', {
          interaction_id: iid,
          decision: decision
        });
      }
      document.getElementById('approval-deny')
        .addEventListener('click', function () { resolveApproval('deny'); });
      document.getElementById('approval-approve')
        .addEventListener('click', function () { resolveApproval('approve'); });
      document.getElementById('approval-always')
        .addEventListener('click', function () { resolveApproval('always_allow'); });

      // ----- inline ask_user_question bubble -----
      function appendQuestionBubble(p) {
        clearPlaceholder();
        var iid     = p.interaction_id || '';
        var qtext   = p.question || '';
        var options = Array.isArray(p.options) ? p.options : [];

        var el = document.createElement('div');
        el.className = 'bubble bubble-question';
        el.setAttribute('data-iid', iid);

        var head = document.createElement('div');
        head.className = 'q-head';
        head.appendChild(document.createTextNode('AI needs your input'));
        el.appendChild(head);

        var qt = document.createElement('div');
        qt.className = 'q-text';
        qt.textContent = qtext;
        el.appendChild(qt);

        if (options.length) {
          var opts = document.createElement('div');
          opts.className = 'q-options';
          options.forEach(function (o) {
            var value, label, desc;
            if (typeof o === 'string') {
              value = label = o; desc = '';
            } else {
              value = o.value || '';
              label = o.label || value;
              desc  = o.description || '';
            }
            var btn = document.createElement('button');
            btn.className = 'q-option';
            btn.innerHTML = '&lt;strong&gt;' + label + '&lt;/strong&gt;' +
              (desc ? '&lt;span class="q-option-desc"&gt;' + desc + '&lt;/span&gt;' : '');
            btn.addEventListener('click', function () {
              answerQuestion(iid, el, value, label);
            });
            opts.appendChild(btn);
          });
          el.appendChild(opts);
        } else {
          // No options → free-text input.
          var row = document.createElement('div');
          row.className = 'q-freetext-row';
          var input = document.createElement('input');
          input.type = 'text';
          input.placeholder = 'Type your answer…';
          var send = document.createElement('button');
          send.className = 'q-option';
          send.textContent = 'Send';
          send.addEventListener('click', function () {
            var v = input.value.trim();
            if (!v) return;
            answerQuestion(iid, el, v, v);
          });
          input.addEventListener('keydown', function (e) {
            if (e.key === 'Enter') { e.preventDefault(); send.click(); }
          });
          row.appendChild(input);
          row.appendChild(send);
          el.appendChild(row);
          setTimeout(function () { input.focus(); }, 50);
        }
        var wasAtBottom = isNearBottom();
        messagesEl.appendChild(el);
        scrollIfNearBottom(wasAtBottom);
      }
      function answerQuestion(iid, bubbleEl, value, displayLabel) {
        sendEnvelope('ask_user_question_response', {
          interaction_id: iid,
          answer: value
        });
        // Lock the bubble visually so the user can't double-click.
        bubbleEl.classList.add('answered');
        bubbleEl.querySelectorAll('button, input').forEach(function (e) { e.disabled = true; });
        var marker = document.createElement('div');
        marker.className = 'q-answered-marker';
        marker.textContent = '✓ Answered: ' + (displayLabel || value);
        bubbleEl.appendChild(marker);
      }

      function handleEnvelope(env) {
        if (!env || !env.t) return;
        if (env.t === 'assistant_chunk') {
          if (!activeBubble) {
            // Text resumed after tool calls (if any) — close the batch so
            // any subsequent tool round becomes a fresh group.
            closeToolBatch();
            // Slash-command replies use a distinct "system" bubble so it's
            // visually clear the response is local and not from the LLM.
            activeBubble = appendBubble(activeTurnIsSlash ? 'system' : 'assistant', '');
          }
          var prior = activeBubble.getAttribute('data-md') || '';
          var next  = prior + ((env.p &amp;&amp; env.p.text) || '');
          activeBubble.setAttribute('data-md', next);
          var contentEl = activeBubble.querySelector('.bubble-content');
          if (contentEl) contentEl.innerHTML = renderMarkdown(next);
          // /slash remote responses tag chunks with remote_status_bubble so
          // the viewer can replace the text with a live QR + URL card.
          if (env.p &amp;&amp; env.p.remote_status_bubble) {
            renderRemoteStatusBubble(activeBubble);
          }
          updateThinking('Streaming response…');
          scrollIfNearBottom();
        } else if (env.t === 'tool_call_start') {
          // Close any open assistant bubble — tool calls always interrupt
          // text streaming.
          activeBubble = null;
          var tname = (env.p &amp;&amp; env.p.name) || 'tool';
          updateThinking('Running ' + tname + '…');
          appendToolBubble(
            tname,
            (env.p &amp;&amp; env.p.args_summary) || '',
            (env.p &amp;&amp; env.p.call_id) || '',
            (env.p &amp;&amp; env.p.args_full) || ''
          );
        } else if (env.t === 'tool_call_done') {
          finalizeToolBubble(
            (env.p &amp;&amp; env.p.call_id) || '',
            !!(env.p &amp;&amp; env.p.ok),
            (env.p &amp;&amp; env.p.summary) || '',
            (env.p &amp;&amp; env.p.summary_full) || ''
          );
          // Tool finished but the turn may continue — go back to a
          // neutral "Thinking…" so the user knows the agent's still
          // deciding what to do next.
          updateThinking('Thinking…');
        } else if (env.t === 'tool_approval_request') {
          showApprovalModal(env.p || {});
        } else if (env.t === 'ask_user_question_request') {
          appendQuestionBubble(env.p || {});
        } else if (env.t === 'interaction_resolved') {
          // Another viewer (phone, desktop, browser) answered this
          // approval or question — clean up our local UI for it. Server
          // already resolved the broker-side promise; this is purely
          // about not leaving a stale modal/active bubble on screen.
          var rp  = env.p || {};
          var rid = rp.interaction_id || '';
          if (rp.kind === 'tool_approval') {
            if (pendingApproval &amp;&amp; pendingApproval.interaction_id === rid) {
              pendingApproval = null;
              document.getElementById('approval-overlay').classList.remove('shown');
            }
          } else if (rp.kind === 'ask_user_question') {
            var qEl = document.querySelector('.bubble-question[data-iid="' + rid + '"]');
            if (qEl &amp;&amp; !qEl.classList.contains('answered')) {
              qEl.classList.add('answered');
              qEl.querySelectorAll('button, input').forEach(function (e) { e.disabled = true; });
              var marker = document.createElement('div');
              marker.className = 'q-answered-marker';
              marker.textContent = '✓ Answered on another device';
              qEl.appendChild(marker);
            }
          }
        } else if (env.t === 'assistant_done') {
          activeBubble = null;
          closeToolBatch();
          finishTurn();
        } else if (env.t === 'state_update') {
          if (env.p &amp;&amp; env.p.key) applyStateUpdate(env.p.key, env.p.value || {});
        } else if (env.t === 'user_message_broadcast') {
          // Another subscriber sent this message. Render it as a user bubble
          // with an author label so every device sees who said what.
          var ubp    = env.p || {};
          var ubText = ubp.text   || '';
          var ubAuth = ubp.author || 'Guest';
          if (ubText) {
            var ubEl = appendBubble('user', ubText);
            // Inject the author label before the text content.
            var ubContent = ubEl.querySelector('.bubble-content');
            if (ubContent) {
              var authSpan = document.createElement('span');
              authSpan.className   = 'bubble-author';
              authSpan.textContent = ubAuth + ':';
              ubContent.insertBefore(authSpan, ubContent.firstChild);
            }
          }
        } else if (env.t === 'error') {
          appendBubble('error', (env.p &amp;&amp; env.p.message) || 'Unknown error');
          activeBubble = null;
          closeToolBatch();
          finishTurn();
        }
      }

      function pollOnce() {
        if (IS_REMOTE) {
          fetch('/api/poll', {
            headers: {
              'Authorization': 'Bearer ' + REMOTE_TOKEN,
              'X-Mraisid':     REMOTE_SID
            },
            credentials: 'same-origin'  // include the mraisid cookie if browser keeps it
          })
          .then(function (r) {
            if (r.status === 401) { onRemoteAuthError(); return null; }
            // Server echoes the sid it's using — if it issued a fresh one
            // (because we sent an empty header on cold-start) pin it.
            var serverSid = r.headers.get('X-Mraisid');
            if (serverSid &amp;&amp; serverSid !== REMOTE_SID) {
              REMOTE_SID = serverSid;
              try { window.localStorage.setItem('modelerai_remote_sid', REMOTE_SID); } catch (e) {}
            }
            return r.text();
          })
          .then(function (body) {
            if (body === null) return;       // 401 path — overlay already shown
            hideRemoteOverlay();              // got a 200 response — we're connected
            if (!body) return;
            try {
              var arr = JSON.parse(body);
              if (Array.isArray(arr)) {
                for (var i = 0; i &lt; arr.length; i++) handleEnvelope(arr[i]);
              }
            } catch (e) { console.warn('poll parse error', e, body); }
          })
          .catch(function (err) { onRemoteNetworkError(err); });
        } else {
          fireFlexsimEvent('modelerAiPoll', '', function (result) {
            if (!result) return;
            try {
              var arr = JSON.parse(result);
              if (Array.isArray(arr)) {
                for (var i = 0; i &lt; arr.length; i++) handleEnvelope(arr[i]);
              }
            } catch (e) { console.warn('poll parse error', e, result); }
          });
        }
      }
      function startPolling() {
        if (pollTimer) return;
        pollTimer = setInterval(pollOnce, 50);
      }
      function stopPolling() {
        if (pollTimer) { clearInterval(pollTimer); pollTimer = null; }
      }

      // -------- Remote-active heartbeat poll --------
      // When the local viewer's fast-poll loop stops between turns, the
      // FlexSim main thread also stops draining the mainthread task queue
      // (drainQueue is called inside modelerAiBridgePoll). That's fine when
      // we're the only client — there's nothing to drain. But when a remote
      // phone is connected, its /api/send requests block on the main thread
      // via mainthread::runAndWait, and a stopped-polling local viewer means
      // those tasks never run → phone "Thinking…" forever.
      //
      // Fix: while the remote server is enabled, run a slow (1s) heartbeat
      // poll independently of the fast-poll loop. This guarantees drainQueue
      // fires often enough to service phone-initiated requests without
      // hammering the local viewer when no turn is active.
      //
      // The fast 50ms poll still wins during active turns; the heartbeat just
      // tops it up between them.
      var remoteHeartbeatTimer = null;
      function startRemoteHeartbeat() {
        if (remoteHeartbeatTimer) return;
        remoteHeartbeatTimer = setInterval(pollOnce, 1000);
      }
      function stopRemoteHeartbeat() {
        if (remoteHeartbeatTimer) { clearInterval(remoteHeartbeatTimer); remoteHeartbeatTimer = null; }
      }
      function finishTurn() {
        activeTurnId = null;
        activeTurnIsSlash = false;
        sendBtnEl.disabled = false;
        inputEl.disabled = false;
        stopBtnEl.disabled = false;
        stopBtnEl.textContent = 'Stop';
        setSendingState(false);
        inputEl.focus();
        // Keep polling a beat after done in case state_updates trail.
        setTimeout(function () { stopPolling(); }, 250);
      }

      // ----- slash autocomplete -----
      // Find a catalog entry by name OR alias. Returns the catalog entry
      // (canonical) or null.
      function findCmd(nameLower) {
        for (var i = 0; i &lt; slashCatalog.length; i++) {
          var c = slashCatalog[i];
          if (c.name === nameLower) return c;
          if ((c.aliases || []).indexOf(nameLower) &gt;= 0) return c;
        }
        return null;
      }
      // Parse the input to figure out what to show. Three cases:
      //   1. Doesn't start with "/" or starts with text-then-space -&gt; close.
      //   2. "/&lt;partial&gt;" (no space yet) -&gt; command-picker mode.
      //   3. "/&lt;known-cmd&gt; [partial-arg]" -&gt; arg-picker mode (only if
      //      the cmd has args).
      function rebuildFilter() {
        var v = inputEl.value;
        if (!v || v.charAt(0) !== '/') { closeSlash(); return; }
        var sp = v.indexOf(' ');
        if (sp &lt; 0) {
          // Command-picker mode.
          slashMode = 'cmd';
          slashActiveCmd = null;
          var q = v.slice(1).toLowerCase();
          var out = [];
          for (var i = 0; i &lt; slashCatalog.length; i++) {
            var c = slashCatalog[i];
            var names = [c.name].concat(c.aliases || []);
            var matched = null;
            for (var j = 0; j &lt; names.length; j++) {
              if (names[j].indexOf(q) === 0) { matched = names[j]; break; }
            }
            if (matched) {
              out.push({
                kind: 'cmd',
                name: c.name,
                description: c.description,
                details: c.details || '',
                usage: c.usage || ('/' + c.name),
                aliases: c.aliases || [],
                hasArgs: !!(c.args &amp;&amp; c.args.choices &amp;&amp; c.args.choices.length),
                _ref: c
              });
            }
          }
          slashFiltered = out;
        } else {
          // Possibly arg-picker mode.
          var cmdName = v.slice(1, sp).toLowerCase();
          var cmd = findCmd(cmdName);
          if (!cmd || !cmd.args || !cmd.args.choices || !cmd.args.choices.length) {
            closeSlash(); return;
          }
          slashMode = 'arg';
          slashActiveCmd = cmd;
          var argQ = v.slice(sp + 1).toLowerCase();
          var argOut = [];
          for (var k = 0; k &lt; cmd.args.choices.length; k++) {
            var ch = cmd.args.choices[k];
            if (!argQ || (ch.value &amp;&amp; ch.value.toLowerCase().indexOf(argQ) === 0)) {
              argOut.push({
                kind: 'arg',
                value: ch.value,
                label: ch.label || ('/' + ch.value),
                description: ch.description || ''
              });
            }
          }
          slashFiltered = argOut;
        }
        if (slashSelectedIdx &gt;= slashFiltered.length) slashSelectedIdx = 0;
        renderPopover();
      }

      // Builds preview+hint markup for the currently-highlighted item.
      // Pure HTML out — both renderPopover (full rebuild) and
      // renderHoverState (in-place update) use this.
      function buildPreviewHtml() {
        var current = slashFiltered[slashSelectedIdx];
        var html = '';
        if (current) {
          if (current.kind === 'cmd') {
            var body = (current.details || current.description || '');
            html = '&lt;div class="slash-preview"&gt;' + body
                 + (current.usage
                     ? '&lt;br&gt;&lt;span class="pv-usage"&gt;' + current.usage + '&lt;/span&gt;'
                     : '')
                 + '&lt;/div&gt;';
          } else {
            html = '&lt;div class="slash-preview"&gt;&lt;strong&gt;' + current.label
                 + '&lt;/strong&gt; — ' + current.description + '&lt;/div&gt;';
          }
        }
        var hintText = (slashMode === 'cmd' &amp;&amp; current &amp;&amp; current.hasArgs)
            ? '↑↓ navigate · Tab to pick options · Enter to run · Esc dismiss'
            : '↑↓ navigate · Tab/Enter to select · Esc dismiss';
        html += '&lt;div id="slash-hint"&gt;' + hintText + '&lt;/div&gt;';
        return html;
      }

      // Full rebuild — used when the FILTERED LIST itself changes
      // (typing in the input, opening the popover). Wipes innerHTML.
      function renderPopover() {
        if (!slashFiltered.length) { closeSlash(); return; }
        popoverEl.innerHTML = '';

        // Header strip — describes the current picker mode.
        var header = document.createElement('div');
        header.className = 'slash-header';
        if (slashMode === 'cmd') {
          header.textContent = 'Slash commands (' + slashFiltered.length + ')';
        } else {
          var ph = (slashActiveCmd &amp;&amp; slashActiveCmd.args &amp;&amp; slashActiveCmd.args.placeholder)
                     || 'argument';
          header.textContent = '/' + slashActiveCmd.name + ' &lt;' + ph + '&gt;';
        }
        popoverEl.appendChild(header);

        var list = document.createElement('div');
        list.className = 'slash-list';
        slashFiltered.forEach(function (it, i) {
          var row = document.createElement('div');
          row.className = 'slash-item' + (i === slashSelectedIdx ? ' selected' : '');
          row.setAttribute('role', 'option');
          row.setAttribute('data-idx', i);

          if (it.kind === 'cmd') {
            row.innerHTML =
              '&lt;span class="slash-name"&gt;/' + it.name + '&lt;/span&gt;' +
              '&lt;span class="slash-desc"&gt;' + it.description + '&lt;/span&gt;' +
              (it.aliases.length
                ? '&lt;span class="slash-aliases"&gt;also: /' + it.aliases.join(' /') + '&lt;/span&gt;'
                : '');
          } else {
            row.innerHTML =
              '&lt;span class="slash-name"&gt;' + it.label + '&lt;/span&gt;' +
              '&lt;span class="slash-desc"&gt;' + it.description + '&lt;/span&gt;';
          }

          // Hover updates the selected index WITHOUT re-rendering the list.
          // Otherwise the innerHTML rewrite resets .slash-list's scroll
          // position to 0, yanking the hovered command out of view.
          row.addEventListener('mouseenter', function () {
            if (slashSelectedIdx === i) return;  // no-op for the row already selected
            slashSelectedIdx = i;
            renderHoverState();
          });
          row.addEventListener('mousedown', function (e) {
            e.preventDefault();
            slashSelectedIdx = i;
            applySlashSelection();
          });
          list.appendChild(row);
        });
        popoverEl.appendChild(list);

        // Preview + hint go in a single sub-tree so renderHoverState can
        // swap them as a unit with one innerHTML write.
        var tail = document.createElement('div');
        tail.className = 'slash-tail';
        tail.innerHTML = buildPreviewHtml();
        popoverEl.appendChild(tail);

        popoverEl.classList.add('open');
      }

      // In-place update for selection changes (hover, arrow keys). Does NOT
      // touch the list DOM, so .slash-list's scroll position is preserved.
      // Only the `.selected` class toggle and the preview/hint sub-tree
      // change.
      function renderHoverState() {
        if (!popoverEl.classList.contains('open')) return;
        var rows = popoverEl.querySelectorAll('.slash-item');
        rows.forEach(function (r) {
          var idx = parseInt(r.getAttribute('data-idx'), 10);
          r.classList.toggle('selected', idx === slashSelectedIdx);
        });
        var tail = popoverEl.querySelector('.slash-tail');
        if (tail) tail.innerHTML = buildPreviewHtml();
        // Keep the selected row visible if arrow-keys move it past the
        // scroll viewport. nearest = don't yank to top/bottom unless needed.
        var sel = popoverEl.querySelector('.slash-item.selected');
        if (sel &amp;&amp; sel.scrollIntoView) sel.scrollIntoView({ block: 'nearest' });
      }

      function closeSlash() {
        popoverEl.classList.remove('open');
        slashFiltered = [];
        slashMode = 'cmd';
        slashActiveCmd = null;
      }

      // While the user arrows through arg choices, mirror the highlighted
      // value into the input so "highlighted = selected" — Enter then just
      // submits whatever's visible. Cmd-mode arrowing leaves the input
      // alone (the user's still typing `/&lt;command&gt;`).
      function previewArgSelection() {
        if (slashMode !== 'arg' || !slashFiltered.length) return;
        var pick = slashFiltered[slashSelectedIdx];
        if (!pick || pick.kind !== 'arg') return;
        var v = inputEl.value;
        var sp = v.indexOf(' ');
        if (sp &lt; 0) return;  // shouldn't happen — arg mode implies a space
        var before = v.slice(0, sp);
        inputEl.value = before + ' ' + pick.value;
        inputEl.selectionStart = inputEl.selectionEnd = inputEl.value.length;
      }

      function applySlashSelection() {
        if (!slashFiltered.length) return;
        var pick = slashFiltered[slashSelectedIdx];
        if (pick.kind === 'cmd') {
          // Inserting a command. If the command has args, transition to
          // arg-picker mode immediately by adding a space.
          inputEl.value = '/' + pick.name + (pick.hasArgs ? ' ' : '');
          inputEl.selectionStart = inputEl.selectionEnd = inputEl.value.length;
          if (pick.hasArgs) {
            // Re-trigger the filter so we render the arg picker.
            rebuildFilter();
          } else {
            closeSlash();
          }
        } else {
          // Inserting an arg choice. Replace the partial arg with the
          // chosen value. No trailing space — user typically wants to
          // hit Enter to run.
          var v = inputEl.value;
          var sp = v.indexOf(' ');
          if (sp &lt; 0) sp = v.length;
          inputEl.value = v.slice(0, sp) + ' ' + pick.value;
          inputEl.selectionStart = inputEl.selectionEnd = inputEl.value.length;
          closeSlash();
        }
        inputEl.focus();
      }

      inputEl.addEventListener('input', rebuildFilter);
      inputEl.addEventListener('focus', rebuildFilter);
      inputEl.addEventListener('blur', function () {
        // Skip the close when the blur was caused by clicking the slash
        // toolbar button — that button's handler will re-focus and open
        // the popover; we don't want to fight it.
        if (slashJustOpenedByButton) return;
        // Delay so the mousedown handler on a popover item fires first.
        setTimeout(closeSlash, 100);
      });
      document.addEventListener('click', function (e) {
        // Slash button is exempt — its own handler opens the popover and
        // we don't want this listener to close it on the same tick.
        if (slashBtnEl &amp;&amp; slashBtnEl.contains(e.target)) return;
        if (!popoverEl.contains(e.target) &amp;&amp; e.target !== inputEl) closeSlash();
      });

      // ----- send -----
      composerEl.addEventListener('submit', function (e) {
        e.preventDefault();
        var text = inputEl.value.trim();
        var hasAttachments = attachments.length &gt; 0;
        // Allow sending with images but no text ("look at this and tell me what you see").
        if ((!text &amp;&amp; !hasAttachments) || activeTurnId) return;
        inputEl.value = '';

        // Snapshot attachments + clear UI state BEFORE any await. Same
        // reasoning as input clearing — visual feedback that the message
        // is on its way.
        var sendingAttachments = attachments.slice();
        attachments = [];
        renderAttachments();

        // Convert each attachment's dataUrl into the wire shape the bridge
        // expects: { name, media_type, data_base64 }. dataUrl is of the
        // form `data:image/png;base64,iVBORw...` — split on the comma to
        // peel off the base64 payload.
        var wireAttachments = sendingAttachments.map(function (a) {
            var du = a.dataUrl || '';
            var comma = du.indexOf(',');
            var mt = '';
            var b64 = '';
            if (comma &gt; 0) {
                // header looks like "data:image/png;base64"
                var header = du.slice(5, comma);                  // strip "data:"
                var semi = header.indexOf(';');
                mt = semi &gt; 0 ? header.slice(0, semi) : header;
                b64 = du.slice(comma + 1);
            }
            return { name: a.name, media_type: mt, data_base64: b64 };
        }).filter(function (a) { return a.media_type &amp;&amp; a.data_base64; });

        appendBubble('user', text, { attachments: sendingAttachments });
        activeTurnId = newId();
        activeTurnIsSlash = (text.charAt(0) === '/');
        sendBtnEl.disabled = true;
        inputEl.disabled = true;
        if (!activeTurnIsSlash) setSendingState(true);
        var envelope = JSON.stringify({
          t: 'user_message', id: activeTurnId,
          p: { text: text, attachments: wireAttachments }
        });
        // dispatchEnvelope handles local-vs-remote routing. Local CEF
        // gets the bridge-error callback (catches "err:no_agent" etc).
        // Remote path ignores the callback — errors come back as `error`
        // envelopes via poll instead, handled in handleEnvelope.
        dispatchEnvelope(envelope, function (status) {
          if (typeof status === 'string' &amp;&amp; status.indexOf('err:') === 0) {
            appendBubble('error', 'Bridge error: ' + status);
            finishTurn();
            return;
          }
        });
      });

      inputEl.addEventListener('keydown', function (e) {
        // If popover is open, capture nav keys first.
        if (popoverEl.classList.contains('open') &amp;&amp; slashFiltered.length) {
          if (e.key === 'ArrowDown') {
            e.preventDefault();
            slashSelectedIdx = (slashSelectedIdx + 1) % slashFiltered.length;
            renderHoverState();
            previewArgSelection();
            return;
          }
          if (e.key === 'ArrowUp') {
            e.preventDefault();
            slashSelectedIdx = (slashSelectedIdx - 1 + slashFiltered.length) % slashFiltered.length;
            renderHoverState();
            previewArgSelection();
            return;
          }
          if (e.key === 'Tab' || (e.key === 'Enter' &amp;&amp; !e.shiftKey)) {
            e.preventDefault();
            applySlashSelection();
            return;
          }
          if (e.key === 'Escape') {
            e.preventDefault();
            closeSlash();
            return;
          }
        }
        var sendOnEnter = document.getElementById('send-on-enter').checked;
        if (sendOnEnter &amp;&amp; e.key === 'Enter' &amp;&amp; !e.shiftKey) {
          e.preventDefault();
          composerEl.dispatchEvent(new Event('submit', { cancelable: true }));
        } else if (!sendOnEnter &amp;&amp; e.key === 'Enter' &amp;&amp; e.ctrlKey) {
          e.preventDefault();
          composerEl.dispatchEvent(new Event('submit', { cancelable: true }));
        }
      });

      // ----- settings: Test key + Open rules + Open log + Copy diagnostics + Reset counters -----
      // Single chokepoint for outbound envelopes. ALL send paths (chat
      // input, mode_change, cancel_turn, viewer_ready, etc.)
      // must funnel through here — otherwise direct fireFlexsimEvent
      // calls leak through in remote mode and hit the no-op mock shim.
      // That bug ate hours; the rule is "if you're sending an envelope,
      // call dispatchEnvelope, not fireFlexsimEvent directly."
      //
      // `envelopeJson` is the full pre-serialized envelope string.
      // `localCallback` (optional) is invoked with the bridgeSend return
      // string for LOCAL CEF only — it lets a caller surface bridge
      // errors like "err:no_agent". In REMOTE mode no equivalent exists
      // (errors come back asynchronously as `error` envelopes via poll),
      // so the callback is ignored on the remote path.
      function dispatchEnvelope(envelopeJson, localCallback) {
        try {
          console.log('[MRAI] dispatchEnvelope',
            { IS_REMOTE: IS_REMOTE, REMOTE_SID: REMOTE_SID, bodyLen: envelopeJson.length });
          if (window.__mraiDiag) {
            window.__mraiDiag.sends.push({ ts: Date.now(),
              IS_REMOTE: IS_REMOTE, bodyLen: envelopeJson.length });
            if (window.__mraiDiag.sends.length &gt; 20) window.__mraiDiag.sends.shift();
          }
        } catch (e) { /* never let diag noise break a real send */ }

        if (IS_REMOTE) {
          fetch('/api/send', {
            method: 'POST',
            headers: {
              'Authorization': 'Bearer ' + REMOTE_TOKEN,
              'Content-Type':  'application/json',
              'X-Mraisid':     REMOTE_SID
            },
            credentials: 'same-origin',
            body: envelopeJson
          })
          .then(function (r) {
            console.log('[MRAI] /api/send response', r.status, r.statusText);
            if (window.__mraiDiag) window.__mraiDiag.lastSendStatus = r.status;
          })
          .catch(function (err) {
            console.error('[MRAI] /api/send failed', err);
            if (window.__mraiDiag) window.__mraiDiag.lastSendError = String(err);
            onRemoteNetworkError(err);
          });
        } else {
          fireFlexsimEvent('modelerAiSend', envelopeJson,
            localCallback || function () {});
        }
        startPolling();
      }

      function sendEnvelope(t, p) {
        var s = JSON.stringify({ t: t, id: newId(), p: p || {} });
        dispatchEnvelope(s);
      }
      var remoteBackoffMs   = 0;
      var remoteRetryTimer  = null;
      function showRemoteOverlay(title, msg, mode) {
        var ov = document.getElementById('remote-disconnect-overlay');
        document.getElementById('remote-disconnect-title').textContent = title;
        document.getElementById('remote-disconnect-msg').textContent   = msg;
        ov.classList.add('show');
        // mode === 'kicked' = static screen (no auto-retry).
        if (mode === 'reconnecting') scheduleRemoteRetry();
      }
      function hideRemoteOverlay() {
        document.getElementById('remote-disconnect-overlay').classList.remove('show');
        if (remoteRetryTimer) { clearTimeout(remoteRetryTimer); remoteRetryTimer = null; }
        remoteBackoffMs = 0;
      }
      function scheduleRemoteRetry() {
        remoteBackoffMs = remoteBackoffMs ? Math.min(30000, remoteBackoffMs * 2) : 1000;
        remoteRetryTimer = setTimeout(function () { pollOnce(); }, remoteBackoffMs);
      }
      function onRemoteAuthError() {
        // Token is bad — don't auto-retry, prompt the user to rescan.
        showRemoteOverlay('Disconnected',
          'Token mismatch. Rescan the QR on the desktop to reconnect.',
          'kicked');
        try { window.localStorage.removeItem('modelerai_remote_token'); } catch (e) {}
      }
      function onRemoteNetworkError(err) {
        showRemoteOverlay('Reconnecting…',
          'Trying to reach the server (backoff ' + Math.round(remoteBackoffMs/1000) + 's).',
          'reconnecting');
      }
      // Manual Retry button wiring — overlay DOM lives above the script
      // tag, so it's parsed by the time this runs.
      var __retryBtn = document.getElementById('remote-disconnect-retry');
      if (__retryBtn) {
        __retryBtn.addEventListener('click', function () {
          hideRemoteOverlay();
          pollOnce();
        });
      }
      // ----- Wire all 4 built-in provider cards through one helper. -----
      // Each card has parallel IDs (&lt;prefix&gt;-test, &lt;prefix&gt;-set, &lt;prefix&gt;-forget,
      // row-&lt;prefix&gt;-keyform, &lt;prefix&gt;-key-input, &lt;prefix&gt;-key-save,
      // &lt;prefix&gt;-key-cancel, &lt;prefix&gt;-badge). Adding a new built-in is a single
      // entry plus the matching HTML block.
      var BUILTIN_PROVIDER_CARDS = [
        { id: 'anthropic', prefix: 'anth',    name: 'Anthropic',     envvar: 'ANTHROPIC_API_KEY' },
        { id: 'openai',    prefix: 'openai',  name: 'OpenAI',        envvar: 'OPENAI_API_KEY'    },
        { id: 'gemini',    prefix: 'gemini',  name: 'Google Gemini', envvar: 'GEMINI_API_KEY'    },
        { id: 'mistral',   prefix: 'mistral', name: 'Mistral',       envvar: 'MISTRAL_API_KEY'   },
        { id: 'xai',       prefix: 'xai',     name: 'xAI Grok',      envvar: 'XAI_API_KEY'       }
      ];
      BUILTIN_PROVIDER_CARDS.forEach(function (p) {
        var testBtn   = document.getElementById(p.prefix + '-test');
        var setBtn    = document.getElementById(p.prefix + '-set');
        var forgetBtn = document.getElementById(p.prefix + '-forget');
        var saveBtn   = document.getElementById(p.prefix + '-key-save');
        var cancelBtn = document.getElementById(p.prefix + '-key-cancel');
        if (!testBtn || !setBtn || !forgetBtn || !saveBtn || !cancelBtn) return;
        testBtn.addEventListener('click', function () {
          var badge = document.getElementById(p.prefix + '-badge');
          if (badge) { badge.className = 'badge badge-warn'; badge.textContent = 'Testing…'; }
          sendEnvelope('test_provider', { provider: p.id });
        });
        setBtn.addEventListener('click', function () {
          var form = document.getElementById('row-' + p.prefix + '-keyform');
          if (!form) return;
          form.style.display = (form.style.display === 'none' ? 'flex' : 'none');
          if (form.style.display === 'flex') {
            var input = document.getElementById(p.prefix + '-key-input');
            if (input) {
              input.value = '';
              setTimeout(function () { input.focus(); }, 50);
            }
          }
        });
        cancelBtn.addEventListener('click', function () {
          var form  = document.getElementById('row-' + p.prefix + '-keyform');
          var input = document.getElementById(p.prefix + '-key-input');
          if (form)  form.style.display = 'none';
          if (input) input.value = '';
        });
        saveBtn.addEventListener('click', function () {
          var input = document.getElementById(p.prefix + '-key-input');
          var k = input ? input.value : '';
          if (!k) { alert('Empty key — not saved.'); return; }
          sendEnvelope('save_api_key', { provider: p.id, key: k });
          var form = document.getElementById('row-' + p.prefix + '-keyform');
          if (form)  form.style.display = 'none';
          if (input) input.value = '';
        });
        forgetBtn.addEventListener('click', function () {
          if (!confirm('Remove the ' + p.name + ' key from the keystore?\n'
                       + 'If ' + p.envvar + ' is set in your environment, '
                       + 'that fallback continues to work.')) return;
          sendEnvelope('forget_api_key', { provider: p.id });
        });
      });
      document.getElementById('model-select').addEventListener('change', function (e) {
        // Bridge looks up the owning provider via findProviderForModel(modelId),
        // so the `provider` field in this envelope is unused — we omit it.
        sendEnvelope('provider_switch', { model: e.target.value });
      });
      document.getElementById('session-open-folder').addEventListener('click', function () {
        var p = window._sessionsDir || '';
        if (!p) { alert('Sessions folder path not yet loaded.'); return; }
        navigator.clipboard &amp;&amp; navigator.clipboard.writeText(p);
        alert('Sessions folder copied to clipboard:\n\n' + p + '\n\nPaste into File Explorer to open. Dedicated open-in-explorer button coming with the keystore UX in v0.3.');
      });
      document.getElementById('session-clear').addEventListener('click', function () {
        if (!confirm('Clear the conversation and delete the on-disk session file?\nThis cannot be undone.')) return;
        sendEnvelope('user_message', { text: '/clear' });
      });
      document.getElementById('session-export').addEventListener('click', function () {
        var btn = this;
        var status = document.getElementById('export-status');
        btn.disabled = true;
        btn.textContent = 'Exporting…';
        if (status) {
          status.style.display = 'block';
          status.textContent   = 'Writing Markdown…';
        }
        sendEnvelope('export_conversation', {});
      });
      document.getElementById('effort-select').addEventListener('change', function (e) {
        sendEnvelope('effort_change', { effort: e.target.value });
      });
      // Persist the General-tab toggles. Both are user-level by convention.
      var soeEl = document.getElementById('send-on-enter');
      if (soeEl) {
        soeEl.addEventListener('change', function () {
          persistSetting('send_on_enter', !!soeEl.checked, 'global');
        });
      }
      var sdiEl = document.getElementById('show-debug');
      if (sdiEl) {
        sdiEl.addEventListener('change', function () {
          persistSetting('show_debug_info', !!sdiEl.checked, 'global');
        });
      }
      // Debug drawer collapse toggle.
      var dbgDrawer = document.getElementById('debug-drawer');
      var dbgHead   = document.getElementById('debug-head');
      var dbgColBtn = document.getElementById('debug-collapse');
      function toggleDbgCollapse() {
        if (!dbgDrawer) return;
        var col = dbgDrawer.classList.toggle('collapsed');
        if (dbgColBtn) dbgColBtn.textContent = col ? '+' : '−';
      }
      if (dbgHead)   dbgHead.addEventListener('click', toggleDbgCollapse);
      if (dbgColBtn) dbgColBtn.addEventListener('click', function (e) {
        e.stopPropagation();
        toggleDbgCollapse();
      });

      // Cost-ceiling inputs (Usage tab). Debounced commit on 'change' so
      // we don't fire a setting write for every keystroke.
      var costWarnEl = document.getElementById('cost-warn');
      var costStopEl = document.getElementById('cost-stop');
      if (costWarnEl) {
        costWarnEl.addEventListener('change', function () {
          var v = parseFloat(costWarnEl.value);
          persistSetting('cost_warn_usd', isNaN(v) ? 0 : v, 'global');
        });
      }
      if (costStopEl) {
        costStopEl.addEventListener('change', function () {
          var v = parseFloat(costStopEl.value);
          persistSetting('cost_stop_usd', isNaN(v) ? 0 : v, 'global');
        });
      }
      var maxIterEl = document.getElementById('max-tool-iterations');
      if (maxIterEl) {
        maxIterEl.addEventListener('change', function () {
          var v = parseInt(maxIterEl.value, 10);
          if (isNaN(v) || v &lt; 1) v = 1;
          if (v &gt; 500) v = 500;
          maxIterEl.value = v;
          persistSetting('max_tool_iterations', v, 'global');
        });
      }
      var exportDirEl = document.getElementById('export-dir');
      if (exportDirEl) {
        exportDirEl.addEventListener('change', function () {
          persistSetting('export_dir', exportDirEl.value.trim(), 'global');
        });
      }
      var resetBtn = document.getElementById('reset-defaults');
      if (resetBtn) {
        resetBtn.addEventListener('click', function () {
          if (!confirm('Reset user preferences to defaults?\n\nThis clears: theme, mode, cost ceiling, export folder, send-on-enter, show-debug-info, session strategy, active model + effort.\n\nIt does NOT clear: API keys, custom providers, terms acceptance, onboarding state, or your audit trail.\n\nContinue?')) return;
          sendEnvelope('reset_settings', { scope: 'global' });
        });
      }

      document.getElementById('cp-add').addEventListener('click', function () {
        var pl = {
          display_name:    document.getElementById('cp-display-name').value.trim(),
          base_url:        document.getElementById('cp-base-url').value.trim(),
          api_key:         document.getElementById('cp-api-key').value,
          model_id:        document.getElementById('cp-model-id').value.trim(),
          model_display:   document.getElementById('cp-model-display').value.trim(),
        };
        var inCost  = parseFloat(document.getElementById('cp-input-cost').value);
        var outCost = parseFloat(document.getElementById('cp-output-cost').value);
        if (!isNaN(inCost))  pl.input_usd_per_mtok  = inCost;
        if (!isNaN(outCost)) pl.output_usd_per_mtok = outCost;

        if (!pl.display_name || !pl.base_url || !pl.model_id) {
          alert('Display name, base URL, and model ID are required.');
          return;
        }

        // Custom-provider acknowledgement gate. Each new third-party
        // provider warrants explicit acceptance: PracSim cannot vet
        // their privacy / training / retention terms — only the user can.
        // The DLL also persists the ack into global settings for an
        // audit trail (custom_provider_acks key).
        var url = pl.base_url.toLowerCase();
        var isDeepSeek = url.indexOf('deepseek') &gt;= 0;
        showCustomProviderAck(pl.display_name, pl.base_url, isDeepSeek, function () {
          var ackPayload = {
            provider_label:  pl.display_name,
            base_url:        pl.base_url,
            accepted_at_ms:  Date.now(),
            warning_level:   isDeepSeek ? 'red' : 'amber'
          };
          // Pass ack along so the DLL logs it.
          pl.acknowledgement = ackPayload;
          sendEnvelope('add_custom_provider', pl);
        });
      });

      // Show the per-provider acknowledgement modal. Caller passes a
      // confirm callback that fires only on positive ack. Two variants:
      // "amber" generic 3rd-party, "red" for known-risky providers
      // (currently just DeepSeek based on the base_url match).
      function showCustomProviderAck(label, baseUrl, isRed, onConfirm) {
        var overlay = document.getElementById('cp-ack-overlay');
        var card    = document.getElementById('cp-ack-card');
        var title   = document.getElementById('cp-ack-title');
        var body    = document.getElementById('cp-ack-body');
        var agreeBtn = document.getElementById('cp-ack-agree');
        var cancelBtn = document.getElementById('cp-ack-cancel');
        var checkbox = document.getElementById('cp-ack-confirm');
        if (!overlay) return;
        card.className = 'cp-ack-card' + (isRed ? ' cp-ack-red' : '');
        title.textContent = isRed
          ? 'Warning — High-Risk Provider'
          : 'Custom Provider — Acknowledge Their Terms';
        if (isRed) {
          body.innerHTML =
            '&lt;p&gt;&lt;strong&gt;You are attempting to add DeepSeek as a provider.&lt;/strong&gt;&lt;/p&gt;' +
            '&lt;p&gt;Public reporting indicates:&lt;/p&gt;' +
            '&lt;ul&gt;' +
              '&lt;li&gt;DeepSeek\'s privacy policy permits use of customer data for model training.&lt;/li&gt;' +
              '&lt;li&gt;Data is stored on servers in the People\'s Republic of China; foreign government access cannot be ruled out.&lt;/li&gt;' +
              '&lt;li&gt;European data-protection regulators (CNIL, Irish DPC) opened investigations in January 2025.&lt;/li&gt;' +
              '&lt;li&gt;A January 2025 ClickHouse database exposure leaked DeepSeek API keys and chat logs.&lt;/li&gt;' +
            '&lt;/ul&gt;' +
            '&lt;p&gt;&lt;strong&gt;Do not send any proprietary, confidential, regulated, or otherwise non-public model data to DeepSeek.&lt;/strong&gt; ' +
            'PracSim Solutions strongly recommends choosing a different provider (Anthropic, OpenAI, or a local runner like Ollama).&lt;/p&gt;' +
            '&lt;p&gt;Endpoint: &lt;code&gt;' + escapeHtml(baseUrl) + '&lt;/code&gt;&lt;/p&gt;';
        } else {
          body.innerHTML =
            '&lt;p&gt;You are about to add &lt;strong&gt;' + escapeHtml(label) + '&lt;/strong&gt; as a custom AI provider.&lt;/p&gt;' +
            '&lt;p&gt;&lt;strong&gt;Your data is governed by their terms, not Anthropic\'s.&lt;/strong&gt; PracSim Solutions has not vetted this provider\'s privacy policy, training behavior, data retention, or compliance posture. Before sending any sensitive, proprietary, confidential, regulated, or otherwise non-public information through this provider you must independently review their:&lt;/p&gt;' +
            '&lt;ul&gt;' +
              '&lt;li&gt;Terms of Service / API agreement&lt;/li&gt;' +
              '&lt;li&gt;Data Processing Addendum (if regulated data is involved)&lt;/li&gt;' +
              '&lt;li&gt;Privacy policy and stated training behavior&lt;/li&gt;' +
              '&lt;li&gt;Data retention and Zero-Data-Retention options&lt;/li&gt;' +
            '&lt;/ul&gt;' +
            '&lt;p&gt;Endpoint: &lt;code&gt;' + escapeHtml(baseUrl) + '&lt;/code&gt;&lt;/p&gt;' +
            '&lt;p&gt;This acknowledgement is logged with a timestamp. You can revoke it by removing the provider in Settings.&lt;/p&gt;';
        }
        checkbox.checked = false;
        agreeBtn.disabled = true;
        checkbox.onchange = function () { agreeBtn.disabled = !checkbox.checked; };
        overlay.classList.add('shown');
        var cleanup = function () {
          overlay.classList.remove('shown');
          agreeBtn.onclick = null;
          cancelBtn.onclick = null;
        };
        cancelBtn.onclick = cleanup;
        agreeBtn.onclick = function () {
          if (!checkbox.checked) return;
          cleanup();
          onConfirm();
        };
      }
      document.getElementById('rules-open').addEventListener('click', function () {
        // v0.2 will add a dedicated open_file envelope; for v0.1, surface
        // the path so the user can paste it into Run / Explorer.
        var path = document.getElementById('rules-path').textContent;
        navigator.clipboard &amp;&amp; navigator.clipboard.writeText(path);
        alert('Path copied to clipboard:\n\n' + path + '\n\nPaste into File Explorer to open. Dedicated open-in-editor button coming in v0.2.');
      });
      document.getElementById('rules-refresh').addEventListener('click', function () {
        sendEnvelope('viewer_ready', {});
      });
      document.getElementById('diag-log').addEventListener('click', function () {
        alert('Bridge log path:\n\n%APPDATA%\\PRACSIM\\ModelerAI\\bridge.log\n\nPaste into File Explorer to open. Dedicated open-in-editor button coming in v0.2.');
      });
      document.getElementById('diag-copy').addEventListener('click', function () {
        var diag = {
          modelerAiVersion: '0.1',
          theme: (effectiveSettings &amp;&amp; effectiveSettings.theme) || 'system',
          userAgent: navigator.userAgent
        };
        var blob = JSON.stringify(diag, null, 2);
        navigator.clipboard &amp;&amp; navigator.clipboard.writeText(blob);
        alert('Diagnostics copied:\n\n' + blob);
      });
      document.getElementById('usage-reset').addEventListener('click', function () {
        sendEnvelope('user_message', { text: '/clear' });
      });

      // ----- session strategy UI -----
      var allSessions = [];

      function updateManualPickerVisibility(strategy) {
        document.getElementById('manual-picker').style.display =
          (strategy === 'manual') ? 'block' : 'none';
      }

      function refreshManualSessionList() {
        sendEnvelope('list_sessions', {});
      }

      // Wire radio inputs. Initial selection reflects legacy localStorage
      // until the DLL pushes the canonical settings (then state_update
      // handler keeps it in sync).
      var legacyStrategy = (function () {
        try { return localStorage.getItem('modelerai.sessionStrategy') || 'per-file'; }
        catch (e) { return 'per-file'; }
      })();
      document.querySelectorAll('input[name="session_strategy"]').forEach(function (r) {
        r.checked = (r.value === legacyStrategy);
        r.addEventListener('change', function () {
          if (!r.checked) return;
          updateManualPickerVisibility(r.value);
          if (r.value === 'manual') refreshManualSessionList();
          persistSetting('session_strategy', r.value, 'global');
        });
      });
      updateManualPickerVisibility(legacyStrategy);

      // Wire manual-session dropdown.
      var manualSelect = document.getElementById('manual-session-select');
      manualSelect.addEventListener('change', function (e) {
        persistSetting('manual_session_key', e.target.value || '', 'global');
      });

      // Wire Save-As behavior radios.
      document.querySelectorAll('input[name="save_as_behavior"]').forEach(function (r) {
        r.addEventListener('change', function () {
          if (!r.checked) return;
          persistSetting('save_as_behavior', r.value, 'global');
        });
      });
      // ----- Session Memory list — new richer view of saved sessions -----
      function formatSavedAt(ms) {
        if (!ms) return '(unknown date)';
        try {
          var d = new Date(ms);
          return d.toISOString().slice(0, 16).replace('T', ' ') + ' UTC';
        } catch (e) { return '(unknown date)'; }
      }
      function formatBytes(b) {
        if (!b) return '0 B';
        if (b &lt; 1024) return b + ' B';
        if (b &lt; 1024 * 1024) return (b / 1024).toFixed(1) + ' KB';
        return (b / (1024 * 1024)).toFixed(1) + ' MB';
      }
      function modelDisplayFromPath(p) {
        if (!p) return '(unknown model)';
        var clean = p.replace(/\\/g, '/');
        var name = clean.split('/').pop();
        return name || clean;
      }
      function renderSessionMemoryList(sessions) {
        var listEl = document.getElementById('sm-list');
        var summaryEl = document.getElementById('sm-summary');
        if (!listEl || !summaryEl) return;

        if (!sessions.length) {
          listEl.innerHTML = '';
          listEl.appendChild(Object.assign(document.createElement('div'), {
            className: 'sm-empty',
            textContent: 'No saved sessions yet. Conversations save automatically once you run /addmodelerai on a model and send a message.'
          }));
          summaryEl.textContent = '0 saved sessions';
          return;
        }

        var totalBytes = 0, totalMsgs = 0;
        sessions.forEach(function (s) {
          totalBytes += (s.bytes || 0);
          totalMsgs  += (s.message_count || 0);
        });
        summaryEl.textContent = sessions.length + ' saved session' + (sessions.length === 1 ? '' : 's')
          + ' · ' + totalMsgs + ' total messages · ' + formatBytes(totalBytes) + ' on disk';

        var currentKey = (document.getElementById('session-key') || {}).textContent || '';
        listEl.innerHTML = '';
        sessions.forEach(function (s) {
          var row = document.createElement('div');
          row.className = 'sm-row' + (s.key === currentKey ? ' current' : '');
          var info = document.createElement('div');
          info.className = 'info';
          var title = document.createElement('div');
          title.className = 'title';
          title.textContent = modelDisplayFromPath(s.model_path);
          var meta = document.createElement('div');
          meta.className = 'meta';
          meta.innerHTML = (s.message_count || 0) + ' messages · '
            + formatBytes(s.bytes || 0) + ' · saved ' + formatSavedAt(s.last_saved_ms)
            + ' · &lt;code&gt;' + (s.key || '?') + '&lt;/code&gt;';
          info.appendChild(title);
          info.appendChild(meta);
          var del = document.createElement('button');
          del.className = 'danger';
          del.textContent = 'Delete';
          del.title = 'Permanently delete this session file.';
          del.addEventListener('click', function () {
            if (!confirm('Delete this saved conversation?\n\n'
                         + modelDisplayFromPath(s.model_path) + '\n'
                         + (s.message_count || 0) + ' messages, '
                         + formatSavedAt(s.last_saved_ms)
                         + '\n\nThis cannot be undone.')) return;
            sendEnvelope('delete_session', { key: s.key });
          });
          row.appendChild(info);
          row.appendChild(del);
          listEl.appendChild(row);
        });
      }

      // Refresh button on the Session Memory panel re-fetches the list.
      var smRefreshBtn = document.getElementById('sm-refresh');
      if (smRefreshBtn) {
        smRefreshBtn.addEventListener('click', refreshManualSessionList);
      }
      var smDeleteAllBtn = document.getElementById('sm-delete-all');
      if (smDeleteAllBtn) {
        smDeleteAllBtn.addEventListener('click', function () {
          if (!confirm('Delete ALL saved Modeler AI conversations on this machine?\n\n'
                       + 'This wipes every session file under '
                       + '%APPDATA%/PRACSIM/ModelerAI/sessions/. The current in-memory '
                       + 'chat keeps running — but the next autosave will write a fresh '
                       + 'file for it.\n\nThis cannot be undone.')) return;
          sendEnvelope('delete_all_sessions', {});
        });
      }

      // ----- one-shot legacy localStorage migration -----
      // Before this commit, theme + session strategy + manual session key
      // lived in CEF localStorage. The new rule is: model or appdata, never
      // CEF. If we find legacy values, send them to the DLL as global
      // settings then clear localStorage so we never migrate again.
      (function migrateLegacyLocalStorage() {
        try {
          var didMigrate = false;
          var theme = localStorage.getItem('modelerai.theme');
          if (theme) {
            persistSetting('theme', theme, 'global');
            localStorage.removeItem('modelerai.theme');
            didMigrate = true;
          }
          var strategy = localStorage.getItem('modelerai.sessionStrategy');
          if (strategy) {
            persistSetting('session_strategy', strategy, 'global');
            localStorage.removeItem('modelerai.sessionStrategy');
            didMigrate = true;
          }
          var custom = localStorage.getItem('modelerai.sessionCustomKey');
          if (custom !== null) {  // empty string also counts
            persistSetting('manual_session_key', custom, 'global');
            localStorage.removeItem('modelerai.sessionCustomKey');
            didMigrate = true;
          }
          if (didMigrate) console.log('[ModelerAI] migrated legacy localStorage settings to global');
        } catch (e) { /* localStorage not available — fine */ }
      })();

      // ----- remote-badge click: jump to Settings → Remote -----
      var rb = document.getElementById('remote-badge');
      if (rb) {
        rb.addEventListener('click', function () {
          var settingsTab = document.querySelector('[data-tab="settings"]');
          if (settingsTab) settingsTab.click();
          var rs = document.querySelector('#settings-sidebar [data-sg="remote"]');
          if (rs) rs.click();
          var rsec = document.querySelector('#settings-content [data-sg="remote"]');
          if (rsec) rsec.scrollIntoView({ behavior: 'smooth' });
        });
      }

      // Diagnostic handle for the browser console. Everything important
      // is normally trapped inside this IIFE; this exposes just enough
      // state to inspect remote-mode bootstrap and a ring of recent send
      // attempts. Type `__mraiDiag` in DevTools to see it; `__mraiDiag
      // .sends` is the history; `__mraiDiag.send('echo')` fires a
      // synthetic send so you can test independent of the UI.
      window.__mraiDiag = {
        IS_REMOTE:        IS_REMOTE,
        REMOTE_TOKEN_len: REMOTE_TOKEN ? REMOTE_TOKEN.length : 0,
        REMOTE_SID:       REMOTE_SID,
        sends:            [],
        lastSendStatus:   null,
        lastSendError:    null,
        // Bypass the input wiring entirely — useful to confirm whether
        // the network path itself is alive when the UI button seems dead.
        send: function (text) {
          sendEnvelope('user_message', { text: text || 'diag test' });
        }
      };
      console.log('[MRAI] viewer bootstrap', window.__mraiDiag);

      // ----- viewer_ready: ask DLL to push initial state -----
      // The LOCAL CEF viewer is authoritative for session-strategy
      // choice — its localStorage is the user's actual preference, so we
      // pass it through. The REMOTE viewer (phone/browser) is a guest:
      // its localStorage is per-device and shouldn't override the desktop
      // user's choice. The server already defends against this (see
      // bridge.cpp viewer_ready handler), but we omit the fields here too
      // so we're not even tempting fate.
      sendEnvelope('viewer_ready', IS_REMOTE ? {} : {
        session_strategy:   getSessionStrategy(),
        custom_session_key: getCustomSessionKey()
      });
      // Pre-fetch the session list so it's ready when the user flips to manual.
      refreshManualSessionList();

      // Always-on slow heartbeat for ALL viewers (local CEF + remote phones).
      // For the local viewer, this guarantees ModelerAi_bridgePoll →
      // mainthread::drainQueue runs at ≥1Hz so phone-initiated /api/send
      // requests (which block on runAndWait) get serviced even when the
      // local fast 50ms poll is idle between turns.
      // For remote phones, this is how they receive cross-subscriber
      // broadcasts (user_message_broadcast, assistant streams from a turn
      // started by someone else, etc.) when they're not in their own turn.
      // Without this, phone queues accumulate envelopes that only get
      // drained when the phone next sends — which is the bug we hit on
      // 2026-06-09 where remote viewers only updated after their own prompt.
      startRemoteHeartbeat();
    })();
  &lt;/script&gt;
&lt;/body&gt;
&lt;/html&gt;
</data></node>
        </node>
        <node f="42"><name>eventfunctions</name>
         <node f="40"><name></name></node>
         <node f="442" dt="2"><name>modelerAiSend</name><data>// JS-callable bridge entry. fireFlexsimEvent must target an event
// function; this shim just dispatches to the modelerAiBridgeSend user
// command (a DLL binding registered as a sibling of addmodelerai).
// No longer depends on the in-model ModelerAI object existing — the
// user command is globally callable as soon as the module loads.
string envelope = param(1);
return applicationcommand("modelerAiBridgeSend", envelope);</data></node>
         <node f="442" dt="2"><name>modelerAiPoll</name><data>// JS-callable bridge entry. Mirrors modelerAiSend — applicationcommand
// dispatch to the modelerAiBridgePoll user command.
return applicationcommand("modelerAiBridgePoll");</data></node>
        </node>
       </data>
        <node f="40"><name></name></node></node>
      </node>
     </node>
    </node>
   </node>
  </node>
  <node f="42" dt="2"><name>add_nodefunctions</name><data>VIEW:/nodefunctions</data>
   <node f="40"><name></name></node>
   <node f="42" dt="3"><name>ModelerAI</name><data><coupling>null</coupling></data>
    <node f="40"><name></name></node>
    <node f="42" dt="1"><name>rank</name><data>0000000000000000</data></node>
    <node f="42" dt="2"><name>after</name><data>Submodels</data></node>
    <node f="42" dt="1"><name>into object</name><data>0000000000000000</data></node>
    <node f="42"><name>data</name>
     <node f="40"><name></name></node>
     <node f="42"><name>ModelerAI</name>
      <node f="40"><name></name></node>
      <node f="442" dt="2"><name>addmodelerai</name><data>treenode activeViewNode = views().find("active&gt;Documents/ModelerAI");
if (activeViewNode.first){
	treenode existingView = ownerobject(tonode(get(rank(activeViewNode, 1))));
	applicationcommand("activatedockedview", existingView);
	return existingView;
}
string viewPath = "VIEW:/modules/ModelerAI/ModelerAI";

treenode modelerAiTools = Model.find("Tools").subnodes.assert("ModelerAI");
treenode modelerAi;
if (!modelerAiTools.first)
	modelerAi = createcopy(library().find("ModelerAI/ModelerAI"), modelerAiTools,1);
else
	modelerAi = modelerAiTools.first;
int skipRefresh = 0;
treenode theView = applicationcommand("createdockedview", viewPath, DOCK_TYPE_DOCUMENT, nodetopath(modelerAi), nodetopath(modelerAi), skipRefresh, [
		[DOCK_PANE_MAIN, "/$findmatchintree(i, get(tabwindow(a)) == DOCK_TYPE_DOCUMENT)$"]
	]);
return theView;</data></node>
      <node f="1000042" dt="2"><name>modelerAiBridgeSend</name><data>dll:"module:ModelerAI" func:"ModelerAi_bridgeSend"</data></node>
      <node f="1000042" dt="2"><name>modelerAiBridgePoll</name><data>dll:"module:ModelerAI" func:"ModelerAi_bridgePoll"</data></node>
      <node f="1000042" dt="2"><name>modelerai_ping</name><data>dll:"module:ModelerAI" func:"ModelerAi_ping"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_object</name><data>dll:"module:ModelerAI" func:"ModelerAi_createObject"</data></node>
      <node f="1000042" dt="2"><name>modelerai_delete_object</name><data>dll:"module:ModelerAI" func:"ModelerAi_deleteObject"</data></node>
      <node f="1000042" dt="2"><name>modelerai_clone_object</name><data>dll:"module:ModelerAI" func:"ModelerAi_cloneObject"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_property</name><data>dll:"module:ModelerAI" func:"ModelerAi_setProperty"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_property</name><data>dll:"module:ModelerAI" func:"ModelerAi_getProperty"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_properties</name><data>dll:"module:ModelerAI" func:"ModelerAi_listProperties"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_label</name><data>dll:"module:ModelerAI" func:"ModelerAi_setLabel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_label</name><data>dll:"module:ModelerAI" func:"ModelerAi_getLabel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_labels</name><data>dll:"module:ModelerAI" func:"ModelerAi_listLabels"</data></node>
      <node f="1000042" dt="2"><name>modelerai_remove_label</name><data>dll:"module:ModelerAI" func:"ModelerAi_removeLabel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_group</name><data>dll:"module:ModelerAI" func:"ModelerAi_createGroup"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_user_command</name><data>dll:"module:ModelerAI" func:"ModelerAi_createUserCommand"</data></node>
      <node f="1000042" dt="2"><name>modelerai_group_add_member</name><data>dll:"module:ModelerAI" func:"ModelerAi_groupAddMember"</data></node>
      <node f="1000042" dt="2"><name>modelerai_group_remove_member</name><data>dll:"module:ModelerAI" func:"ModelerAi_groupRemoveMember"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_groups</name><data>dll:"module:ModelerAI" func:"ModelerAi_listGroups"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_group_members</name><data>dll:"module:ModelerAI" func:"ModelerAi_listGroupMembers"</data></node>
      <node f="1000042" dt="2"><name>modelerai_delete_group</name><data>dll:"module:ModelerAI" func:"ModelerAi_deleteGroup"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_global_table</name><data>dll:"module:ModelerAI" func:"ModelerAi_createGlobalTable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_resize_global_table</name><data>dll:"module:ModelerAI" func:"ModelerAi_resizeGlobalTable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_global_table_cell</name><data>dll:"module:ModelerAI" func:"ModelerAi_setGlobalTableCell"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_global_table_cell</name><data>dll:"module:ModelerAI" func:"ModelerAi_getGlobalTableCell"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_global_tables</name><data>dll:"module:ModelerAI" func:"ModelerAi_listGlobalTables"</data></node>
      <node f="1000042" dt="2"><name>modelerai_delete_global_table</name><data>dll:"module:ModelerAI" func:"ModelerAi_deleteGlobalTable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_parameter</name><data>dll:"module:ModelerAI" func:"ModelerAi_getParameter"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_parameters</name><data>dll:"module:ModelerAI" func:"ModelerAi_listParameters"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_classes</name><data>dll:"module:ModelerAI" func:"ModelerAi_listClasses"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_object_info</name><data>dll:"module:ModelerAI" func:"ModelerAi_getObjectInfo"</data></node>
      <node f="1000042" dt="2"><name>modelerai_extract_class_schema</name><data>dll:"module:ModelerAI" func:"ModelerAi_extractClassSchema"</data></node>
      <node f="1000042" dt="2"><name>modelerai_connect_fixed_resources</name><data>dll:"module:ModelerAI" func:"ModelerAi_connectFixedResources"</data></node>
      <node f="1000042" dt="2"><name>modelerai_connect_task_executer_to_navigator</name><data>dll:"module:ModelerAI" func:"ModelerAi_connectTaskExecuterToNavigator"</data></node>
      <node f="1000042" dt="2"><name>modelerai_connect_fixed_resource_to_navigator</name><data>dll:"module:ModelerAI" func:"ModelerAi_connectFixedResourceToNavigator"</data></node>
      <node f="1000042" dt="2"><name>modelerai_connect_dispatcher_to_task_executer</name><data>dll:"module:ModelerAI" func:"ModelerAi_connectDispatcherToTaskExecuter"</data></node>
      <node f="1000042" dt="2"><name>modelerai_connect_centerport</name><data>dll:"module:ModelerAI" func:"ModelerAi_connectCenterport"</data></node>
      <node f="1000042" dt="2"><name>modelerai_disconnect</name><data>dll:"module:ModelerAI" func:"ModelerAi_disconnect"</data></node>
      <node f="1000042" dt="2"><name>modelerai_inspect_connections</name><data>dll:"module:ModelerAI" func:"ModelerAi_inspectConnections"</data></node>
      <node f="1000042" dt="2"><name>modelerai_add_parameter</name><data>dll:"module:ModelerAI" func:"ModelerAi_addParameter"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_parameter</name><data>dll:"module:ModelerAI" func:"ModelerAi_setParameter"</data></node>
      <node f="1000042" dt="2"><name>modelerai_remove_parameter</name><data>dll:"module:ModelerAI" func:"ModelerAi_removeParameter"</data></node>
      <node f="1000042" dt="2"><name>modelerai_notify_run_state</name><data>dll:"module:ModelerAI" func:"ModelerAi_notifyRunState"</data></node>
      <node f="1000042" dt="2"><name>modelerai_reset_model</name><data>dll:"module:ModelerAI" func:"ModelerAi_resetModel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_run_to_time</name><data>dll:"module:ModelerAI" func:"ModelerAi_runToTime"</data></node>
      <node f="1000042" dt="2"><name>modelerai_run_to_end</name><data>dll:"module:ModelerAI" func:"ModelerAi_runToEnd"</data></node>
      <node f="1000042" dt="2"><name>modelerai_run_until</name><data>dll:"module:ModelerAI" func:"ModelerAi_runUntil"</data></node>
      <node f="1000042" dt="2"><name>modelerai_run</name><data>dll:"module:ModelerAI" func:"ModelerAi_run"</data></node>
      <node f="1000042" dt="2"><name>modelerai_stop_model</name><data>dll:"module:ModelerAI" func:"ModelerAi_stopModel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_step_model</name><data>dll:"module:ModelerAI" func:"ModelerAi_stepModel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_run_state</name><data>dll:"module:ModelerAI" func:"ModelerAi_getRunState"</data></node>
      <node f="1000042" dt="2"><name>modelerai_add_stop_time</name><data>dll:"module:ModelerAI" func:"ModelerAi_addStopTime"</data></node>
      <node f="1000042" dt="2"><name>modelerai_remove_stop_time</name><data>dll:"module:ModelerAI" func:"ModelerAi_removeStopTime"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_warmup_time</name><data>dll:"module:ModelerAI" func:"ModelerAi_setWarmupTime"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_run_speed</name><data>dll:"module:ModelerAI" func:"ModelerAi_setRunSpeed"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_performance_measure</name><data>dll:"module:ModelerAI" func:"ModelerAi_createPerformanceMeasure"</data></node>
      <node f="1000042" dt="2"><name>modelerai_delete_performance_measure</name><data>dll:"module:ModelerAI" func:"ModelerAi_deletePerformanceMeasure"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_performance_measures</name><data>dll:"module:ModelerAI" func:"ModelerAi_listPerformanceMeasures"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_performance_measure</name><data>dll:"module:ModelerAI" func:"ModelerAi_getPerformanceMeasure"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_performance_measures</name><data>dll:"module:ModelerAI" func:"ModelerAi_getPerformanceMeasures"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_object_stats</name><data>dll:"module:ModelerAI" func:"ModelerAi_getObjectStats"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_model_summary</name><data>dll:"module:ModelerAI" func:"ModelerAi_getModelSummary"</data></node>
      <node f="1000042" dt="2"><name>modelerai_export_tree_json</name><data>dll:"module:ModelerAI" func:"ModelerAi_ExportTreeJson"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_trigger</name><data>dll:"module:ModelerAI" func:"ModelerAi_setTrigger"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_trigger</name><data>dll:"module:ModelerAI" func:"ModelerAi_getTrigger"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_triggers</name><data>dll:"module:ModelerAI" func:"ModelerAi_listTriggers"</data></node>
      <node f="1000042" dt="2"><name>modelerai_remove_trigger</name><data>dll:"module:ModelerAI" func:"ModelerAi_removeTrigger"</data></node>
      <node f="1000042" dt="2"><name>modelerai_find_objects</name><data>dll:"module:ModelerAI" func:"ModelerAi_findObjects"</data></node>
      <node f="1000042" dt="2"><name>modelerai_validate_model</name><data>dll:"module:ModelerAI" func:"ModelerAi_validateModel"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_statistics_collector</name><data>dll:"module:ModelerAI" func:"ModelerAi_createStatisticsCollector"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_tracked_variable</name><data>dll:"module:ModelerAI" func:"ModelerAi_createTrackedVariable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_create_processflow</name><data>dll:"module:ModelerAI" func:"ModelerAi_createProcessFlow"</data></node>
      <node f="1000042" dt="2"><name>modelerai_open_processflow_view</name><data>dll:"module:ModelerAI" func:"ModelerAi_openProcessFlowView"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_processflows</name><data>dll:"module:ModelerAI" func:"ModelerAi_listProcessFlows"</data></node>
      <node f="1000042" dt="2"><name>modelerai_delete_processflow</name><data>dll:"module:ModelerAI" func:"ModelerAi_deleteProcessFlow"</data></node>
      <node f="1000042" dt="2"><name>modelerai_add_activity</name><data>dll:"module:ModelerAI" func:"ModelerAi_addActivity"</data></node>
      <node f="1000042" dt="2"><name>modelerai_connect_activities</name><data>dll:"module:ModelerAI" func:"ModelerAi_connectActivities"</data></node>
      <node f="1000042" dt="2"><name>modelerai_delete_activity</name><data>dll:"module:ModelerAI" func:"ModelerAi_deleteActivity"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_activity_variable</name><data>dll:"module:ModelerAI" func:"ModelerAi_setActivityVariable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_list_activities</name><data>dll:"module:ModelerAI" func:"ModelerAi_listActivities"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_activity_info</name><data>dll:"module:ModelerAI" func:"ModelerAi_getActivityInfo"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_activity_variable</name><data>dll:"module:ModelerAI" func:"ModelerAi_getActivityVariable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_activity_table_cell</name><data>dll:"module:ModelerAI" func:"ModelerAi_setActivityTableCell"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_activity_table_cell</name><data>dll:"module:ModelerAI" func:"ModelerAi_getActivityTableCell"</data></node>
      <node f="1000042" dt="2"><name>modelerai_get_activity_table_size</name><data>dll:"module:ModelerAI" func:"ModelerAi_getActivityTableSize"</data></node>
      <node f="1000042" dt="2"><name>modelerai_resize_activity_table</name><data>dll:"module:ModelerAI" func:"ModelerAi_resizeActivityTable"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_activity_table_column_header</name><data>dll:"module:ModelerAI" func:"ModelerAi_setActivityTableColumnHeader"</data></node>
      <node f="1000042" dt="2"><name>modelerai_set_create_object_target_label</name><data>dll:"module:ModelerAI" func:"ModelerAi_setCreateObjectTargetLabel"</data></node>
     </node>
    </node>
   </node>
  </node>
  <node f="42" dt="2"><name>add_tools</name><data>VIEW:/standardviews/modelingutilities/Toolbox&gt;variables/tools</data>
   <node f="40"><name></name></node>
   <node f="42" dt="3"><name>ModelerAI</name><data><coupling>null</coupling></data>
    <node f="40"><name></name></node>
    <node f="42" dt="1"><name>rank</name><data>0000000000000000</data></node>
    <node f="42" dt="2"><name>after</name><data>Workspaces</data></node>
    <node f="42" dt="1"><name>into object</name><data>0000000000000000</data></node>
    <node f="42"><name>data</name>
     <node f="40"><name></name></node>
     <node f="42" dt="4"><name>ModelerAI</name><data>
      <node f="40"><name></name></node>
      <node f="42" dt="2"><name>objectfocus</name><data>MODEL:/Tools/ModelerAI</data></node>
      <node f="42" dt="2"><name>bitmap</name><data>modules\ModelerAI\ModelerAI.png</data></node>
      <node f="42"><name>variables</name>
       <node f="40"><name></name></node>
       <node f="42" dt="2"><name>menuBitmap</name><data>modules\ModelerAI\ModelerAI.png</data></node>
       <node f="42" dt="2"><name>toolType</name><data>Singleton</data></node>
       <node f="42"><name>addMenu</name>
        <node f="40"><name></name></node>
        <node f="2000000042" dt="2"><name>ModelerAI</name><data>applicationcommand("addmodelerai")</data></node>
       </node>
      </node>
      <node f="42"><name>eventfunctions</name>
       <node f="40"><name></name></node>
       <node f="442" dt="2"><name>onDoubleClick</name><data>applicationcommand("addmodelerai")</data></node>
       <node f="442" dt="2"><name>onDelete</name><data>return 1;</data></node>
      </node>
      <node f="4000000042" dt="2"><name>windowtitle</name><data>ModelerAI</data></node>
     </data></node>
    </node>
   </node>
  </node>
  <node f="42" dt="2"><name>add_library</name><data>MAIN:/project/library</data>
   <node f="40"><name></name></node>
   <node f="42" dt="3"><name>ModelerAI</name><data><coupling>null</coupling></data>
    <node f="40"><name></name></node>
    <node f="42" dt="1"><name>rank</name><data>0000000000000000</data></node>
    <node f="42" dt="2"><name>after</name><data>fluid</data></node>
    <node f="42" dt="1"><name>into object</name><data>0000000000000000</data></node>
    <node f="42"><name>data</name>
     <node f="40"><name></name></node>
     <node f="42"><name>ModelerAI</name>
      <node f="40"><name></name></node>
      <node f="10000042" dt="4"><name>ModelerAI</name><data>
       <node f="40"><name></name></node>
       <node f="42"><name>variables</name>
        <node f="40"><name></name></node>
        <node f="42" dt="2"><name>projectSettings</name><data></data></node>
       </node>
       <node f="42"><name>classes</name>
        <node f="40"><name></name></node>
        <node f="42" dt="3"><name>ModelerAI::ModelerAI</name><data><coupling>/installdata/add_library/ModelerAI/data/ModelerAI/ModelerAI</coupling></data></node>
       </node>
      </data></node>
     </node>
    </node>
   </node>
  </node>
  <node f="42" dt="2"><name>add_Documents</name><data>VIEW:/active&gt;Documents</data>
   <node f="40"><name></name></node>
   <node f="42" dt="3"><name>ModelerAI</name><data><coupling>null</coupling></data>
    <node f="40"><name></name></node>
    <node f="42" dt="1"><name>rank</name><data>0000000000000000</data></node>
    <node f="42" dt="2"><name>after</name><data>Html</data></node>
    <node f="42" dt="1"><name>into object</name><data>0000000000000000</data></node>
    <node f="42"><name>data</name>
     <node f="40"><name></name></node>
     <node f="42"><name>ModelerAI</name>
      <node f="40"><name></name></node></node>
    </node>
   </node>
  </node>
  <node f="42" dt="2"><name>add_Tools</name><data>MAIN:/project/exec/globals/Tools</data>
   <node f="40"><name></name></node>
   <node f="42" dt="3"><name>ModelerAI</name><data><coupling>null</coupling></data>
    <node f="40"><name></name></node>
    <node f="42" dt="1"><name>rank</name><data>0000000000000000</data></node>
    <node f="42" dt="2"><name>after</name><data>SubmodelImport</data></node>
    <node f="42" dt="1"><name>into object</name><data>0000000000000000</data></node>
    <node f="42"><name>data</name>
     <node f="40"><name></name></node>
     <node f="42"><name>ModelerAI</name>
      <node f="40"><name></name></node>
      <node f="42" dt="2"><name>LibraryPath</name><data>ModelerAI</data></node>
      <node f="42" dt="2"><name>FolderName</name><data>ModelerAI</data></node>
     </node>
    </node>
   </node>
  </node>
 </node>
 <node f="42" dt="2"><name>release</name><data>26.0</data></node>
 <node f="42" dt="2"><name>revision</name><data>.0</data></node>
 <node f="42" dt="2"><name>flexsim release</name><data>26.0</data></node>
</node></flexsim-tree>
