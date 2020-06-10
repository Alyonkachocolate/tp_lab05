#include <gtest/gtest.h>

#include <Account.h>

TEST(Account, constructor) {
  ASSERT_NO_THROW(Account(57, 5));
  EXPECT_EQ(Account(57, 5).id(), 57);
  EXPECT_EQ(Account(57, 5).GetBalance(), 5);
}

TEST(Account, for_id) {
  EXPECT_EQ(Account(712, 56).id(), 712);
  Account account(418, 142);
  EXPECT_EQ(account.id(), 418);
}

TEST(Account, for_balance) {
  EXPECT_EQ(Account(712, 56).GetBalance(), 56);
  Account account(418, 142);
  EXPECT_EQ(account.GetBalance(), 142);
  EXPECT_EQ(account.GetBalance(), 142);
}

TEST(Account, under_lock) {
  Account account(39, 256);
  account.Lock();

  EXPECT_EQ(account.id(), 39);
  EXPECT_EQ(account.GetBalance(), 256);
}

TEST(Account, ChangeBalance) {
  Account account(39, 101);
  account.Lock();

  account.ChangeBalance(0);
  EXPECT_EQ(account.GetBalance(), 101);

  account.ChangeBalance(102);
  EXPECT_EQ(account.GetBalance(), 203);

  account.ChangeBalance(-1);
  EXPECT_EQ(account.GetBalance(), 202);
}

TEST(Account, ChangeBalance_without_lock) {
  Account account(39, 101);
  EXPECT_EQ(account.GetBalance(), 101);

  ASSERT_THROW(account.ChangeBalance(0), std::runtime_error);
  ASSERT_THROW(account.ChangeBalance(102), std::runtime_error);
  ASSERT_THROW(account.ChangeBalance(-1), std::runtime_error);

  account.Lock();
  account.ChangeBalance(102);
  EXPECT_EQ(account.GetBalance(), 203);

  account.Unlock();
  ASSERT_THROW(account.ChangeBalance(-1), std::runtime_error);
}

TEST(Account, Lock) {
  Account account(4322, 23123);
  ASSERT_NO_THROW(account.Lock());
  ASSERT_THROW(account.Lock(), std::runtime_error);
  ASSERT_NO_THROW(account.Unlock());
  ASSERT_NO_THROW(account.Lock());
}

TEST(Account, UnLock) {
  Account account(4322, 23123);
  ASSERT_NO_THROW(account.Unlock());
  ASSERT_NO_THROW(account.Lock());
}
