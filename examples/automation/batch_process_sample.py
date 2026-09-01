#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
VectorPDF — Python CLI Toplu İşlem (Batch Processing) Örnek Betiği
Bu betik, bir klasördeki PDF dosyalarını tarayarak VectorPDF toplu işlem kuyruğuna
aktarılmak üzere JSON formatında görev manifesti üretir.
"""

import os
import sys
import json
import uuid
from datetime import datetime

def generate_batch_jobs(input_dir, output_dir, action="Optimize"):
    if not os.path.exists(input_dir):
        print(f"Hata: Girdi dizini bulunamadı: {input_dir}")
        return []

    if not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)

    pdf_files = [f for f in os.listdir(input_dir) if f.lower().endswith('.pdf')]
    print(f"Tespit edilen PDF dosya sayısı: {len(pdf_files)}")

    jobs = []
    for filename in pdf_files:
        in_path = os.path.join(input_dir, filename)
        out_path = os.path.join(output_dir, f"processed_{filename}")
        job = {
            "jobId": str(uuid.uuid4()),
            "type": action,
            "inputFilePath": os.path.abspath(in_path),
            "outputFilePath": os.path.abspath(out_path),
            "status": "Pending",
            "progressPercentage": 0,
            "retryCount": 0,
            "maxRetries": 3,
            "createdAt": datetime.now().isoformat()
        }
        jobs.append(job)
        print(f"  [+] Kuyruğa Eklendi: {filename} -> {action}")

    return jobs

def main():
    input_directory = os.path.join("tests", "fixtures")
    output_directory = "output"
    action_type = "ValidatePDFA"

    print("==========================================")
    print("   VectorPDF Python Toplu İşlem Betiği   ")
    print("==========================================")

    job_queue = generate_batch_jobs(input_directory, output_directory, action_type)
    
    manifest_path = "batch_queue_manifest.json"
    with open(manifest_path, "w", encoding="utf-8") as f:
        json.dump({"totalJobs": len(job_queue), "jobs": job_queue}, f, indent=2, ensure_ascii=False)

    print(f"\nToplam {len(job_queue)} iş için manifest oluşturuldu: {manifest_path}")

if __name__ == "__main__":
    main()
