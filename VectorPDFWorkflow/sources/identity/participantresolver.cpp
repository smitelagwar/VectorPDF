// MIT License
#include "participantresolver.h"
#include <QRegularExpression>

namespace vectorpdf::workflow
{

WorkflowParticipant ParticipantResolver::currentLocalUser()
{
    WorkflowParticipant p;
    p.name = QStringLiteral("Yerel Kullanıcı");
    p.email = QStringLiteral("user@local.device");
    p.role = ParticipantRole::Reviewer;
    return p;
}

bool ParticipantResolver::validateEmail(const QString& email)
{
    QRegularExpression re(QStringLiteral(r"^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$"));
    return re.match(email.trimmed()).hasMatch();
}

} // namespace vectorpdf::workflow
