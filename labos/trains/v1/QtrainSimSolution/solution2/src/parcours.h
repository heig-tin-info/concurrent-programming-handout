#ifndef PARCOURS_H
#define PARCOURS_H

#include "sharedsection.h"
#include "sharedsectionsettings.h"

class Parcours
{
public:
    Parcours(std::vector<int> contacts, std::pair<SharedSectionSettings, SharedSectionSettings> settings, std::shared_ptr<SharedSectionInterface> sharedSection) :
        sharedSection(sharedSection),  settings(settings), contacts(contacts) {}

    std::shared_ptr<SharedSectionInterface> sharedSection;
    std::pair<SharedSectionSettings, SharedSectionSettings> settings;
    std::vector<int> contacts;
};

#endif // PARCOURS_H
