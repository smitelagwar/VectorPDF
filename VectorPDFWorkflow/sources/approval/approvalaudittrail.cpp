// MIT License
#include "approvalaudittrail.h"

namespace vectorpdf::workflow
{

QStringList ApprovalAuditTrail::generateAuditLog(const ApprovalDefinition& def)
{
    QStringList log;
    log << QStringLiteral("=== VECTORPDF İŞ AKIŞI DENETİM İZİ ===");
    log << QStringLiteral("İş Akışı ID: ") + def.metadata.workflowId.toString();
    log << QStringLiteral("Oluşturulma: ") + def.metadata.createdAt.toString(Qt::ISODate);

    for (const auto& s : def.steps)
    {
        QString decStr = (s.decision == ApprovalDecision::Approved) ? QStringLiteral("ONAYLANDI") :
                         (s.decision == ApprovalDecision::Rejected) ? QStringLiteral("REDDEDİLDİ") : QStringLiteral("BEKLİYOR");
        log << QStringLiteral("Adım %1: %2 - %3 (%4)").arg(s.order).arg(s.approverName, decStr, s.decisionTimestamp.toString(Qt::ISODate));
    }
    return log;
}

} // namespace vectorpdf::workflow
