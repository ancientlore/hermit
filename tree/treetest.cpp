// Test Tree Code

#include <cstring>
#include <cstdio>
#include "tree/tree.h"
#include "tree_tpl.h"
#include <crtdbg.h> // in borland?

using namespace std;

class String : public Sortable
{
public:

  const char *str;

  String(const char *s) : str(s) {}
  virtual ~String() {}

  virtual int compare(const Sortable& obj) const;
};

int String::compare(const Sortable& obj) const
{
  return strcmp(str, ((const String&) obj).str);
}

void Printer(const Sortable& o)
{
  printf("%-8.8s", ((String&) o).str);
}

int main()
{
  {
  Tree t(0);

  String s[15] = {
    "Dharmesh", "Andy", "Sriram", "John", "Daniel",
    "Michael", "Yadira", "Awez", "Robert", "Greg",
    "Steve", "Glen", "Rahul", "Piotr", "Gigi"
//    "Ranae", "Liz", "Edith", "Kevin", "Lore",
//    "VW", "Buick", "Ford", "Honda", "Nissan"
  };

  for (int i = 0; i < 15; i++)
  {
    if (t.add(&s[i]) == 0)
    {
      printf("Error adding element <%s>\n", s[i].str);
      return 1;
    }
  }

  t.printTree(Printer, 8);


  try {
    TreeIterator iter(t);

    while (!iter.done())
    {
      const String *o = (const String *) (const Sortable *) iter;

      puts(o->str);

      iter++;
    }
  }
  catch (const exception& e) {
    printf ("%s\n", e.what());
  }

  TTree<int> tt;

  tt.add (1);
  tt.add (2);

  try {
      TTreeIterator<int> iter (tt);

    while (!iter.done())
    {
      int i = iter;

      printf ("i=%d\n", i);

      iter++;
    }
  }
  catch (const exception& e) {
    printf ("%s\n", e.what());
  }
  }

  _CrtDumpMemoryLeaks ();

  return 0;
}

// End
