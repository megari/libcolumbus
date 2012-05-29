/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESULTGATHERER_HH_
#define RESULTGATHERER_HH_

/**
 * This class stores the data gathered during query execution.
 */

struct ResultGathererPrivate;
class Word;
class IndexMatches;

class ResultGatherer {
private:

    ResultGathererPrivate *p;

public:
    ResultGatherer();
    ~ResultGatherer();

    void addMatches(const Word &queryWord, const Word &indexName, IndexMatches &matches);
};

#endif /* RESULTGATHERER_HH_ */
