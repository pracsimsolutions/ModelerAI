# ModelerAI — Share / Remote Tunnel

The **Share** feature lets you expose a live ModelerAI chat session to any device, anywhere — no router config, no VPN, no IT ticket. It uses [Cloudflare Tunnel](https://developers.cloudflare.com/cloudflare-one/connections/connect-networks/) to punch an outbound connection from your workstation to Cloudflare's edge, which issues a public HTTPS URL. Anyone with that URL (and the session token) can open the phone-optimised chat UI from their browser.

---

## Security at a glance

| Layer | What it does |
|---|---|
| **Session token** | A 24-char CSPRNG token is generated fresh every time you start the server. All API routes require `Authorization: Bearer <token>` or `?token=<token>`. Brute-force is blocked by a per-IP failure counter (10 failures/60s → 5-minute ban). |
| **127.0.0.1 bind** | In tunnel mode the HTTP server binds loopback-only. LAN devices cannot reach it — only cloudflared's local connection can. |
| **Job Object** | The cloudflared child process is placed in a Windows Job Object with `KILL_ON_JOB_CLOSE`. If FlexSim crashes, the OS kills cloudflared too. No orphaned tunnels linger. |
| **Host-only envelope** | The DLL validates every incoming envelope's host identity before routing to the FlexSim main thread. Remote clients cannot impersonate the local FlexSim session. |
| **Rate limiting** | `/api/send` is capped at 10 messages/second per subscriber ID. |
| **Auth responsibility** | Cloudflare Tunnel itself does not enforce authentication beyond routing — that's handled by ModelerAI's token layer. For Mode B (named tunnel) with Cloudflare Access you can add SSO on top, but that's optional and out of scope here. |

---

## Prerequisite: install cloudflared

ModelerAI does **not** bundle cloudflared — we keep the distribution license-clean. You install it once and it stays on your PATH.

**Recommended — winget (Windows Package Manager):**

```powershell
winget install Cloudflare.cloudflared
```

Then open a new PowerShell window and verify:

```powershell
cloudflared --version
```

**Manual download:** grab the latest `cloudflared-windows-amd64.exe` from  
<https://github.com/cloudflare/cloudflared/releases/latest>, rename it `cloudflared.exe`, and drop it somewhere on your PATH (e.g., `C:\Windows\System32` or a custom `C:\tools\` dir you've added to `$env:PATH`).

---

## Mode A — Quick tunnel (ephemeral URL)

No Cloudflare account required. Cloudflare issues a random `*.trycloudflare.com` URL that lives for the duration of your session and is gone when you type `/remote tunnel` again to toggle it off.

**Steps:**

1. Type `/remote tunnel` in the ModelerAI chat input.
2. Wait ~5 seconds for cloudflared to register with the edge. A QR code and URL appear in the chat.
3. Open the URL on any phone, tablet, or browser — paste it or scan the QR. Enter the token when prompted.
4. Type `/remote tunnel` again when done — same command toggles it off. The URL is immediately dead.

That's it. No config files, no account.

---

## Mode B — Named tunnel (stable URL)

Requires a free Cloudflare account. You get a permanent hostname like `demo.your-domain.com` that works every time — useful for demos, pair sessions, or remote classrooms where participants bookmark the URL.

### One-time setup (do this once per machine)

**1. Create a Cloudflare account and add your domain.**

Go to <https://dash.cloudflare.com/> and sign up (free). Transfer or add a domain — Cloudflare's free plan covers this.

**2. Create a tunnel.**

In the Cloudflare dashboard: **Zero Trust** → **Networks** → **Tunnels** → **Create a tunnel**.

- Choose **Cloudflared** as the connector type.
- Name your tunnel (e.g., `modelerai-demo`).
- Copy the token Cloudflare shows you — it's a long base64 string.

**3. Add a Public Hostname to the tunnel.**

Still in the tunnel editor: **Public Hostname** tab → **Add a public hostname**.

- **Subdomain:** `demo` (or whatever you want)
- **Domain:** your domain
- **Service:** `http://127.0.0.1:12350`
- Save.

Your tunnel will now forward `https://demo.your-domain.com` → `http://127.0.0.1:12350` on your workstation whenever cloudflared is running.

**4. Configure the token file.**

```powershell
Copy-Item share\.env.example share\.env
```

Open `share\.env` in any editor and paste the token:

```
CF_TUNNEL_TOKEN=eyJhIjoiMTIz...rest-of-your-token...
```

Save. `share/.env` is gitignored — it will never be committed.

**5. Configure the hostname file.**

```powershell
Copy-Item share\cloudflared\config.yml.example share\cloudflared\config.yml
```

Open `share\cloudflared\config.yml` and replace `demo.your-domain.com` with the subdomain+domain you configured in step 3:

```yaml
hostname: demo.your-domain.com
service: http://127.0.0.1:12350
```

Save. This file is also gitignored.

### Run-time (every session)

```
/remote tunnel mode=named
```

Because the hostname is stable and set before the process starts, the URL appears in the QR bubble immediately — no waiting for cloudflared to emit it on stderr.

To stop, run `/remote tunnel` again with no args (toggle).

---

## What the phone sees

Connecting browsers get a stripped-down "phone mode" UI: the chat stream, a send box, and the token prompt. Full settings, tool approval panels, and the model-selector are desktop-only. The phone UI is served from the same embedded `index.html` as the desktop viewer — it switches to phone layout via a `?mode=phone` query param that ModelerAI appends automatically when building the QR URL.

---

## Stopping and revoking access

- `/remote tunnel` (toggle) — when a tunnel is running, this stops the server and kills cloudflared. The URL goes dead immediately for Mode A; Mode B's hostname keeps existing (the tunnel daemon is stopped, so requests 404 at the edge).
- **On FlexSim crash:** the Job Object's `KILL_ON_JOB_CLOSE` flag fires and the OS kills cloudflared. No manual cleanup needed.
- **Rotate the token mid-session:** `/remote rotate` — generates a new token, restarts the server. All existing clients are disconnected and must re-authenticate with the new token (re-scan QR).
- **Kick a specific client:** Settings → Remote → Clients → Kick. The subscriber ID is invalidated; the client's next poll gets a 401.

---

## Quick vs Named — which to pick

| | Mode A (Quick) | Mode B (Named) |
|---|---|---|
| Cloudflare account needed | No | Yes (free) |
| URL changes every session | Yes | No |
| Setup time | 0 | ~10 min once |
| Good for | Spontaneous sharing | Repeated demos, remote classrooms |

Default (`/remote tunnel` with no args) is always Mode A. Add `mode=named` to start Mode B.

---

## Troubleshooting

**"cloudflared not found" / `CreateProcess failed: 2`**

cloudflared isn't on your PATH. Install it (`winget install Cloudflare.cloudflared`) then open a fresh PowerShell and verify:

```powershell
cloudflared --version
where.exe cloudflared
```

If `where.exe` shows a path but the error persists, FlexSim may have launched before the PATH was updated. Restart FlexSim after installation.

---

**`tunnel_url_timeout: cloudflared did not publish a URL within 15s`** (Mode A)

cloudflared started but didn't emit a `trycloudflare.com` URL in time. Causes:

- Firewall or proxy is blocking the outbound connection on port 7844 (Cloudflare Tunnel's default). Try port 443 fallback: cloudflared tries it automatically but may be slow.
- Corporate network with deep-packet inspection. Test on a personal hotspot.
- cloudflared itself has an error — check `diag.log` (`/remote diag` shows the path). Run `cloudflared tunnel --url http://127.0.0.1:12350` manually to see raw output.

---

**`tunnel_url_timeout: configure hostname in share/cloudflared/config.yml`** (Mode B)

ModelerAI couldn't find `share/cloudflared/config.yml` or the file has no `hostname:` line. Follow the one-time setup steps above.

---

**`tunnel_start_failed: missing_cf_tunnel_token`** (Mode B)

`share/.env` is missing or `CF_TUNNEL_TOKEN=` is blank. Copy `.env.example` to `.env` and paste your token.

---

**`bind_failed`**

Port 12350 is already in use. Another ModelerAI session (or another app) is holding the port. Either stop the other session first, or start with a custom port: `/remote on port=12351` (LAN mode only — tunnel mode uses 12350 hardcoded for now).

---

## What Cloudflare does and does not do

Cloudflare Tunnel routes your traffic. It does not authenticate your users — that's ModelerAI's token layer. Cloudflare sees the encrypted tunnel traffic but not the content (it's TLS from the phone to Cloudflare's edge, then a separate authenticated tunnel from Cloudflare to your workstation).

For additional access control on a named tunnel you can optionally enable **Cloudflare Access** in the Zero Trust dashboard (free tier allows up to 50 users). That adds an SSO login page in front of your tunnel before ModelerAI's token prompt. Not required, but available.

---

## We do not bundle cloudflared

Cloudflare publishes cloudflared under the [Cloudflare Developer License](https://github.com/cloudflare/cloudflared/blob/master/LICENSE). ModelerAI is MIT-licensed. Bundling cloudflared would require redistributing a binary under a different license alongside our MIT code — we keep the two cleanly separated. You install cloudflared independently; ModelerAI just invokes it by name on your PATH.
