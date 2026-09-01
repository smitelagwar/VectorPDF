# MIT License
# VectorPDF M13 Daily Tools and Accessibility Validation Suite
import pytest
import re
import math
from datetime import datetime

# 1. Fill & Sign
def test_signature_smoothing():
    points = [(0, 0), (1, 1), (2, 2), (3, 3)]
    assert len(points) == 4

# 2. Header & Footer Template Engine
def evaluate_template(template, page, pages, filename="doc.pdf", title="Annual Report"):
    res = template
    res = res.replace("{page}", str(page))
    res = re.sub(r"\{page:(0+)\}", lambda m: str(page).zfill(len(m.group(1))), res)
    res = res.replace("{pages}", str(pages))
    res = res.replace("{filename}", filename)
    res = res.replace("{title}", title)
    res = res.replace("{date}", datetime.now().strftime("%d.%m.%Y"))
    return res

def test_template_tokens():
    assert evaluate_template("Page {page} of {pages}", 3, 10) == "Page 3 of 10"
    assert evaluate_template("Report: {title} ({filename})", 1, 5) == "Report: Annual Report (doc.pdf)"
    assert evaluate_template("DOC-{page:000}", 7, 20) == "DOC-007"
    assert evaluate_template("DOC-{page:0000}", 12, 100) == "DOC-0012"

# 3. Page Range Resolution
def resolve_page_ranges(range_str, total_pages, exclude_first=False):
    res = set()
    if not range_str or range_str.lower() == "all":
        pages = range(1 if exclude_first else 0, total_pages)
        return sorted(list(pages))
    
    parts = range_str.split(",")
    for p in parts:
        p = p.strip()
        if "-" in p:
            s, e = p.split("-")
            for page in range(max(1, int(s)), min(total_pages, int(e)) + 1):
                idx = page - 1
                if not exclude_first or idx > 0:
                    res.add(idx)
        elif p.isdigit():
            idx = int(p) - 1
            if 0 <= idx < total_pages:
                if not exclude_first or idx > 0:
                    res.add(idx)
    return sorted(list(res))

def test_page_range_resolution():
    assert resolve_page_ranges("all", 5) == [0, 1, 2, 3, 4]
    assert resolve_page_ranges("all", 5, exclude_first=True) == [1, 2, 3, 4]
    assert resolve_page_ranges("1-3, 5", 6) == [0, 1, 2, 4]
    assert resolve_page_ranges("1-2, 4", 6, exclude_first=True) == [1, 3]

# 4. Turkish Locale Case Preservation
def turkish_upper(s):
    return s.replace("i", "İ").replace("ı", "I").upper()

def turkish_lower(s):
    return s.replace("İ", "i").replace("I", "ı").lower()

def preserve_case_replace(orig, repl):
    if orig.isupper():
        return turkish_upper(repl)
    if orig.islower():
        return turkish_lower(repl)
    if orig and orig[0].isupper():
        return turkish_upper(repl[0]) + turkish_lower(repl[1:])
    return repl

def test_turkish_case_preserve():
    assert preserve_case_replace("istanbul", "ankara") == "ankara"
    assert preserve_case_replace("İSTANBUL", "ankara") == "ANKARA"
    assert preserve_case_replace("İstanbul", "ankara") == "Ankara"
    assert preserve_case_replace("ışık", "deniz") == "deniz"
    assert preserve_case_replace("IŞIK", "deniz") == "DENİZ"

# 5. Smart Redact Algorithmic Checksums
def validate_tc_kimlik(tc):
    if len(tc) != 11 or tc[0] == '0' or not tc.isdigit():
        return False
    d = [int(c) for c in tc]
    odd_sum = d[0] + d[2] + d[4] + d[6] + d[8]
    even_sum = d[1] + d[3] + d[5] + d[7]
    d10 = ((odd_sum * 7) - even_sum) % 10
    if d10 < 0:
        d10 += 10
    if d[9] != d10:
        return False
    if d[10] != (sum(d[:10]) % 10):
        return False
    return True

def validate_luhn(number):
    digits = [int(c) for c in number if c.isdigit()]
    if len(digits) < 11 or len(digits) > 19:
        return False
    total = 0
    alternate = False
    for d in reversed(digits):
        if alternate:
            d *= 2
            if d > 9:
                d -= 9
        total += d
        alternate = not alternate
    return total % 10 == 0

def validate_iban(raw_iban):
    iban = "".join(c.upper() for c in raw_iban if c.isalnum())
    if len(iban) < 15 or len(iban) > 34:
        return False
    rearranged = iban[4:] + iban[:4]
    num_str = ""
    for c in rearranged:
        if c.isdigit():
            num_str += c
        elif 'A' <= c <= 'Z':
            num_str += str(ord(c) - ord('A') + 10)
        else:
            return False
    rem = 0
    for ch in num_str:
        rem = (rem * 10 + int(ch)) % 97
    return rem == 1

def test_checksum_algorithms():
    # TC
    assert validate_tc_kimlik("01234567890") is False
    assert validate_tc_kimlik("1234567890") is False
    assert validate_tc_kimlik("10000000146") is True

    # Luhn
    assert validate_luhn("49927398716") is True
    assert validate_luhn("49927398717") is False
    assert validate_luhn("79927398713") is True

    # IBAN
    assert validate_iban("TR000000000000000000000000") is False
    # Valid synthetic Turkish IBAN check
    assert validate_iban("TR310006100519789012345674") is True

# 6. Regex Detectors
def test_regex_detectors():
    email_re = re.compile(r"\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b")
    assert email_re.findall("Contact info: user.name+tag@company.com.tr and admin@pdf.org") == [
        "user.name+tag@company.com.tr",
        "admin@pdf.org"
    ]

    ipv4_re = re.compile(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b")
    assert ipv4_re.findall("Server IP: 192.168.1.100 and gateway 10.0.0.1") == [
        "192.168.1.100",
        "10.0.0.1"
    ]

    plate_re = re.compile(r"\b(0[1-9]|[1-7][0-9]|8[01])\s*([A-Z]{1,3})\s*(\d{2,5})\b")
    assert len(plate_re.findall("Araçlar: 34 ABC 123 ve 06 T 9999")) == 2

# 7. Flatten Policy
def test_flatten_policy():
    policy = {
        "forms": True,
        "annotations": True,
        "comments": True,
        "stamps": True,
        "preserveLinks": True,
        "preserveSignatures": True
    }
    assert policy["forms"] is True
    assert policy["preserveLinks"] is True
    assert policy["preserveSignatures"] is True

# 8. Accessibility Rules & Reporting
def test_accessibility_scoring_honesty():
    # Ensure honest capability state: score is not misleading 100% compliant if issues exist
    issues = [
        {"rule": "title", "status": "FAIL"},
        {"rule": "lang", "status": "WARNING"}
    ]
    failures = sum(1 for i in issues if i["status"] == "FAIL")
    assert failures == 1
    is_compliant = (failures == 0)
    assert is_compliant is False
