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

#ifndef INDEXWEIGHTS_HH_
#define INDEXWEIGHTS_HH_

struct IndexWeightsPrivate;
class Word;

class IndexWeights {
    IndexWeightsPrivate *p;
public:
    IndexWeights();
    ~IndexWeights();

    void setWeight(const Word &w, double weigth);
    double getWeight(const Word &w) const;
};

#endif /* INDEXWEIGHTS_HH_ */