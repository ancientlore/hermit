/* ---------------------------------------------------------------------------
  Sortable class

  Object to be placed in AVL Trees and other classes.

  Copyright (C) 1993 Michael D. Lore
  All Rights Reserved.
--------------------------------------------------------------------------- */

#ifndef SORTABLE_H
#define SORTABLE_H


class Sortable
{
public:

  Sortable() {}
  virtual ~Sortable() {}

  virtual int compare(const Sortable& obj) const = 0;
};

inline int operator == (const Sortable& a, const Sortable& b)
{
  return (a.compare(b) == 0);
}

inline int operator != (const Sortable& a, const Sortable& b)
{
  return (a.compare(b) != 0);
}

inline int operator < (const Sortable& a, const Sortable& b)
{
  return (a.compare(b) < 0);
}

inline int operator > (const Sortable& a, const Sortable& b)
{
  return (a.compare(b) > 0);
}

inline int operator >= (const Sortable& a, const Sortable& b)
{
  return (a.compare(b) >= 0);
}

inline int operator <= (const Sortable& a, const Sortable& b)
{
  return (a.compare(b) <= 0);
}


#endif

/* End */
