#include <stdio.h>
#include <bits/stdc++.h>
#include <tuple>

using namespace std;

#define WRITE 'W'
#define READ 'R'
#define COMMIT 'C'

// defines Transaction struct
typedef struct T
{
  int timestamp;
  int id;
  int index;

  char op;
  char att;

} T;

// auxiliary functions

void print_vector_elem(T t)
{
  printf("{ timestamp: %d id:%d operation:%c attribute:%c index:%d }\n", t.timestamp, t.id, t.op, t.att, t.index);
}

void print_vector(vector<T> v)
{

  for (size_t i = 0; i < v.size(); i++)
  {
    print_vector_elem(v[i]);
  }
}

void read_input(vector<T> *v)
{
  int input_1, input_2, index = 0;
  char input_3, input_4;

  while (scanf("%d %d  %c  %c", &input_1, &input_2, &input_3, &input_4) != EOF)
  {
    T escalation;

    escalation.timestamp = input_1;
    escalation.id = input_2;
    escalation.op = input_3;
    escalation.att = input_4;
    escalation.index = index;
    index++;

    v->push_back(escalation);
  }
}

int *max(int *a, int *b) { return (*a > *b) ? a : b; }

// Escalation Algorithms

set<tuple<int, int, bool>> test_seriability(vector<T> transactions)
{

  set<int> nodes;

  set<tuple<int, char>> writes;
  set<tuple<int, char>> reads;

  for (size_t i = 0; i < transactions.size(); i++)
  {
    if (transactions[i].op != COMMIT)
    {
      nodes.insert(transactions[i].id);
    }
  }

  int graph[nodes.size() + 1][nodes.size() + 1];

  for (int i = 1; i < (int)nodes.size() + 1; i++)
  {
    for (int j = 1; j < (int)nodes.size() + 1; j++)
    {
      graph[i][j] = 0;
    }
  }

  for (int i = 0; i < (int)transactions.size(); i++)
  {

    T t = transactions[i];
    if (t.op == READ)
    {
      // cout << "reading " << t.id << "\n";

      for (auto it = writes.begin(); it != writes.end(); ++it)
      {
        if (get<1>(*it) == t.att && get<0>(*it) != t.id)
        {
          graph[t.id][get<0>(*it)]++;
        }
      }
      reads.insert(make_tuple(t.id, t.att));
    }
    else if (t.op == WRITE)
    {
      // cout << "writing " << t.id << "\n";

      for (auto it = writes.begin(); it != writes.end(); ++it)
      {
        if (get<1>(*it) == t.att && get<0>(*it) != t.id)
        {
          graph[t.id][get<0>(*it)]++;
        }
      }

      for (auto it = reads.begin(); it != reads.end(); ++it)
      {

        if (get<1>(*it) == t.att && get<0>(*it) != t.id)
        {
          graph[t.id][get<0>(*it)]++;
        }
      }

      writes.insert(make_tuple(t.id, t.att));
    }
    else if (t.op == COMMIT)
    {
      // cout << "commiting " << t.id << "\n";

      for (auto it = writes.begin(); it != writes.end();)
      {

        if (get<0>(*it) == t.id)
        {
          writes.erase(it++);
        }
        else
        {
          it++;
        }
      }
      for (auto it = reads.begin(); it != reads.end();)
      {

        if (get<0>(*it) == t.id)
        {
          reads.erase(it++);
        }
        else
        {
          it++;
        }
      }
    }
  }

  set<tuple<int, int, bool>> escalations;

  for (int i = 1; i < (int)nodes.size() + 1; i++)
  {
    for (int j = 1; j < (int)nodes.size() + 1; j++)
    {
      if (graph[i][j] > 0)
      {
        if (graph[j][i] > 0)
        {
          bool already_on_set = false;
          for (auto it = escalations.begin(); it != escalations.end(); ++it)
          {
            if (get<0>(*it) == j && get<1>(*it) == i)
            {
              already_on_set = true;
            }
          }
          if (!already_on_set)
          {
            escalations.insert(make_tuple(i, j, false));
          }
        }
        else
        {
          escalations.insert(make_tuple(i, j, true));
        }
      }
    }
  }
  return escalations;
}
void print(const std::vector<int> &v)
{
  for (int e : v)
  {
    std::cout << " " << e;
  }
  std::cout << std::endl;
}

void equivalent_vision(vector<T> transactions)
{
  set<int> escalations;
  for (size_t i = 0; i < transactions.size(); i++)
  {
    if (transactions[i].op != COMMIT)
    {
      escalations.insert(transactions[i].id);
    }
  }
  vector<int> vectorized_escalations(escalations.begin(), escalations.end());
  int count = 0;
  do
  {
    vector<T> current_permutation;

    for (int e : vectorized_escalations)
    {
      for (T t : transactions)
      {
        if (t.id == e)
        {
          current_permutation.push_back(t);
        }
      }
    }

    for (T t : current_permutation)
    {
      set<tuple<int, char>> writes;
      if (t.op == READ)
      {
      }
      else if (t.op == WRITE)
      {
      }
      else if (t.op == COMMIT)
    }
    cout << "permutation: " << count << endl;
    print_vector(current_permutation);
    cout << '\n';
    count++;
  } while (std::next_permutation(vectorized_escalations.begin(), vectorized_escalations.end()));
}

int main(int argc, char const *argv[])
{

  vector<T> transactions;

  read_input(&transactions);

  set<tuple<int, int, bool>> seriability_escalations = test_seriability(transactions);
  equivalent_vision(transactions);

  // for (auto it = escalations.begin(); it != escalations.end(); ++it)
  // {
  //   cout << get<0>(*it) << "," << get<1>(*it) << "," << get<2>(*it) << "\n";
  // }

  // // print_vector(transactions);
}
