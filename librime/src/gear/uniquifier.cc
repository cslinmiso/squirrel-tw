//
// Copyleft 2011 RIME Developers
// License: GPLv3
//
// 2011-12-12 GONG Chen <chen.sst@gmail.com>
//
#include <rime/candidate.h>
#include <rime/common.h>
#include <rime/engine.h>
#include <rime/gear/uniquifier.h>

namespace rime {

// Uniquifier

Uniquifier::Uniquifier(Engine *engine) : Filter(engine) {
}

void Uniquifier::Apply(CandidateList *recruited,
                       CandidateList *candidates) {
  if (!candidates || candidates->empty())
    return;
  CandidateList::iterator i = candidates->begin();
  while (i != candidates->end()) {
    CandidateList::iterator j = recruited->begin();
    for (; j != recruited->end(); ++j) {
      if ((*i)->text() == (*j)->text()) {
        shared_ptr<UniquifiedCandidate> c = As<UniquifiedCandidate>(*j);
        if (!c) {
          c = make_shared<UniquifiedCandidate>(*j, "uniquified");
          *j = c;
        }
        c->Append(*i);
        break;
      }
    }
    if (j == recruited->end())
      ++i;
    else
      i = candidates->erase(i);
  }
}

}  // namespace rime
