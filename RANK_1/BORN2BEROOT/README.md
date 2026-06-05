# Born2beRoot

> Rank 01 · École 42 Common Core — *Introduction to virtualization & system administration*

## Why there is (almost) no code here
Born2beRoot is **not a programming project** — it is a system-administration
exercise. The deliverable is a properly configured **virtual machine**, not a
source tree. The only thing pushed to the git repository is a **`signature.txt`**
file containing the SHA-256 signature of the VM's virtual disk
(`.vdi`/`.qcow2`). During defense the evaluator regenerates the signature from
your disk and checks it matches the one committed here — this proves the
submitted VM is exactly the one being graded.

That is why this folder only contains `signature.txt`.

## What the project was
Set up and harden a headless server from scratch inside a VM:

- Install **Debian** (or Rocky Linux) with **no graphical interface** —
  everything is done from the command line.
- Partition the disk using **LVM**, with at least **two encrypted partitions**.
- Configure **SSH** to run on **port 4242 only**, with **root login disabled**.
- Enable a firewall (**UFW**/firewalld) that leaves **only port 4242 open**.
- Enforce a strict **password policy** and a hardened **`sudo`** configuration
  (limited to 3 attempts, custom error message, full command logging to
  `/var/log/sudo/`, `secure_path`, etc.).
- Write a **`monitoring.sh`** script (run periodically via `cron`) that
  broadcasts system stats with `wall`: architecture & kernel, physical/virtual
  CPU count, RAM and disk usage, CPU load, last boot, LVM status, active
  connections, user count, IPv4/MAC address, and the number of `sudo` commands run.

## How it was completed
The VM was installed and configured manually following the subject. The hostname
was set to `<login>42`, a strong root and user password were chosen, the user was
added to the `sudo` and `user42` groups, and the monitoring script was scheduled.
Once everything passed, the VM disk was signed and that signature committed here.

## Constraints (2025 subject)
- No graphical environment; Debian is the recommended distro (Rocky allowed).
- Mandatory: LVM with at least 2 encrypted partitions, UFW with only 4242 open,
  SSH on 4242 without root login, strong password policy, hardened `sudo`,
  and the monitoring script.
- The committed disk signature must match the evaluated VM exactly.

*Subject reference: 42 `Born2beRoot` subject (2024–2025 edition).*
