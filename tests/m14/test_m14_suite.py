# MIT License
# VectorPDF M14 Collaboration, Review, Forensics, Scan & Recovery Validation Suite
import pytest
import json
import hashlib
from datetime import datetime

# 1. Recovery Manifest & Journal
def serialize_recovery_manifest(session_id, original_path, actions_count):
    return {
        "sessionId": session_id,
        "originalFilePath": original_path,
        "uncommittedActionsCount": actions_count,
        "state": "Active",
        "lastAutosaveTime": datetime.now().isoformat()
    }

def test_recovery_manifest():
    meta = serialize_recovery_manifest("sess-123", "C:/docs/contract.pdf", 4)
    assert meta["sessionId"] == "sess-123"
    assert meta["uncommittedActionsCount"] == 4

def parse_journal_log(log_text):
    entries = []
    for line in log_text.strip().splitlines():
        parts = line.split("|")
        if len(parts) >= 3:
            entries.append({"index": int(parts[0]), "timestamp": parts[1], "action": parts[2]})
    return entries

def test_recovery_journal_parsing():
    raw_log = "1|2026-09-01T10:00:00|AddStamp|ONAYLANDI\n2|2026-09-01T10:01:00|ApplyBates|CASE-001"
    parsed = parse_journal_log(raw_log)
    assert len(parsed) == 2
    assert parsed[0]["action"] == "AddStamp"
    assert parsed[1]["action"] == "ApplyBates"

# 2. Workflow Package Manifest & State
def test_workflow_package_manifest():
    pkg = {
        "workflowId": "wf-999",
        "type": "SignatureRequest",
        "title": "Vendor NDA",
        "participants": [
            {"name": "Alice", "email": "alice@corp.com", "role": "Signer", "hasCompleted": True},
            {"name": "Bob", "email": "bob@corp.com", "role": "Signer", "hasCompleted": False}
        ]
    }
    completed = sum(1 for p in pkg["participants"] if p["hasCompleted"])
    progress = (completed / len(pkg["participants"])) * 100.0
    assert progress == 50.0

# 3. Approval Workflow Audit Trail
def test_approval_workflow():
    steps = [
        {"order": 1, "approver": "Legal", "decision": "Approved"},
        {"order": 2, "approver": "Finance", "decision": "Approved"}
    ]
    is_fully_approved = all(s["decision"] == "Approved" for s in steps)
    assert is_fully_approved is True

    steps.append({"order": 3, "approver": "CEO", "decision": "Pending"})
    is_fully_approved_now = all(s["decision"] == "Approved" for s in steps)
    assert is_fully_approved_now is False

# 4. Bates Numbering Multi-Document Continuity
def generate_bates_numbers(prefix, start_num, digits, suffix, count):
    return [f"{prefix}{str(start_num + i).zfill(digits)}{suffix}" for i in range(count)]

def test_bates_numbering_continuity():
    doc1_bates = generate_bates_numbers("CASE-", 1, 6, "", 3)
    assert doc1_bates == ["CASE-000001", "CASE-000002", "CASE-000003"]

    doc2_bates = generate_bates_numbers("CASE-", 4, 6, "", 2)
    assert doc2_bates == ["CASE-000004", "CASE-000005"]

# 5. Sanitize Before Sharing
def test_sanitize_policy_completeness():
    policy = {
        "removeMetadata": True,
        "removeAttachments": True,
        "removeJavaScript": True,
        "removeActions": True,
        "removeHiddenLayers": True
    }
    assert all(policy.values()) is True

# 6. Advanced Print Imposition (N-Up & Booklet)
def calculate_n_up(total_pages, n_up):
    sheets = []
    cur = []
    for p in range(total_pages):
        cur.append(p)
        if len(cur) == n_up:
            sheets.append(cur)
            cur = []
    if cur:
        sheets.append(cur)
    return sheets

def test_n_up_imposition():
    sheets = calculate_n_up(5, 2)
    assert sheets == [[0, 1], [2, 3], [4]]

def calculate_booklet(total_pages):
    padded = (total_pages + 3) // 4 * 4
    sheets = []
    left = 0
    right = padded - 1
    while left < right:
        sheets.append((right if right < total_pages else None, left if left < total_pages else None))
        left += 1
        right -= 1
        sheets.append((left if left < total_pages else None, right if right < total_pages else None))
        left += 1
        right -= 1
    return sheets

def test_booklet_imposition():
    booklet_8 = calculate_booklet(8)
    assert len(booklet_8) == 4
    assert booklet_8[0] == (7, 0) # Back page & Front page

# 7. Blank Page Detection Metrics
def is_blank_page(dark_pixel_ratio, threshold=0.5):
    return dark_pixel_ratio < threshold

def test_blank_page_detection():
    assert is_blank_page(0.04) is True
    assert is_blank_page(12.5) is False

# 8. Duplicate Page Perceptual Hashing (dHash & Hamming)
def hamming_distance(h1, h2):
    return bin(h1 ^ h2).count("1")

def similarity_score(h1, h2):
    dist = hamming_distance(h1, h2)
    return (64.0 - dist) / 64.0

def test_duplicate_fingerprint():
    hash1 = 0b1010101010101010101010101010101010101010101010101010101010101010
    hash2 = 0b1010101010101010101010101010101010101010101010101010101010101010 # Identical
    hash3 = 0b1010101010101010101010101010101010101010101010101010101010101011 # 1 bit difference

    assert similarity_score(hash1, hash2) == 1.0
    assert similarity_score(hash1, hash3) == 63.0 / 64.0
    assert similarity_score(hash1, hash3) > 0.95 # Classifies as duplicate
