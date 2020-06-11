#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Account.h>
#include <Transaction.h>

using ::testing::AtLeast;
using ::testing::Return;

class Mock_Account : public Account {
public:
  Mock_Account(int id, int balance) : Account(id, balance) {}

  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};

TEST(Transaction, Make) {
  Transaction transaction;
  Mock_Account account1(1, 123), account2(1, 456);

  // id=id
  ASSERT_THROW(transaction.Make(account1, account2, 15), std::logic_error);

  // negative
  Mock_Account account3(1, 123), account4(2, 456);
  ASSERT_THROW(transaction.Make(account3, account4, -123),
               std::invalid_argument);

  // small sum
  account3.ChangeBalance(700);
  account4.ChangeBalance(3000);
  ASSERT_THROW(transaction.Make(account3, account4, 99), std::logic_error);
}

TEST(Transaction, fee) {
  Transaction transaction;

  transaction.set_fee(255);
  ASSERT_EQ(transaction.fee(), 255);
}

TEST(Transaction, transaction_too_small_sum) {
  Transaction transaction;
  transaction.set_fee(100);

  Mock_Account account1(5, 1000), account2(7, 1000);
  for (int i = 100; i < 2 * 100; ++i)
    ASSERT_FALSE(transaction.Make(account1, account2, i));
}

TEST(Transaction, transaction_not_successful) {
  Transaction transaction;
  transaction.set_fee(200);

  Mock_Account account1(5, 1000), account2(7, 1000);
  ASSERT_FALSE(transaction.Make(account1, account2, 5000));
  ASSERT_FALSE(transaction.Make(account2, account1, 2000));
}

TEST(Transaction, transaction_successful) {
  Transaction transaction;
  transaction.set_fee(200);

  Mock_Account account1(5, 1000), account2(7, 1000);
  ASSERT_FALSE(transaction.Make(account1, account2, 5000));
  ASSERT_FALSE(transaction.Make(account2, account1, 2000));

  EXPECT_CALL(account1, Lock()).Times(1).WillOnce([&account1]() {
    return account1.Account::Lock();
  });
  EXPECT_CALL(account1, ChangeBalance(-(600 + 200))).Times(1).WillOnce
  ([&account1]() {
        return account1.Account::ChangeBalance(-(600 + 200));
      });
  EXPECT_CALL(account1, Unlock()).Times(1).WillOnce([&account1]() {
    return account1.Account::Unlock();
  });

  EXPECT_CALL(account2, Lock()).Times(1).WillOnce([&account2]() {
    return account2.Account::Lock();
  });
  EXPECT_CALL(account2, ChangeBalance(600)).Times(1).WillOnce([&account2]() {
    return account2.Account::ChangeBalance(600);
  });
  EXPECT_CALL(account2, Unlock()).Times(1).WillOnce([&account2]() {
    return account2.Account::Unlock();
  });

  ASSERT_TRUE(transaction.Make(account1, account2, 600));
}